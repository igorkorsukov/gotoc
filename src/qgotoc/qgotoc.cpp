#include "qgotoc.h"
#include <QDebug>
#include <QElapsedTimer>

#include "gotoc.h"

typedef void(*C_call)(int, void*, int, void**, int*);

QGoToC::QGoToC()
{
    C_call p = &QGoToC::static_c_call;
    GoToC_Init(reinterpret_cast<void*>(p));

    m_fns[10] = &m_onEcho;
}

QGoToC::~QGoToC()
{
    qDeleteAll(m_wraps);
}

QGoToC::Err QGoToC::reg(int cFi, CFnRaw *fn)
{
    if (cFi < 100) {
        return Err(QString("gotoc: fi < 100 are reserved, fi: %1").arg(cFi));
    }

    if (m_fns.contains(cFi)) {
        return Err(QString("gotoc: already registered fi: %1").arg(cFi));
    }

    m_fns[cFi] = fn;
    return Err();
}

QGoToC::Err QGoToC::reg(int cFi, CFn *fn)
{
    FnWrap *w = new FnWrap();
    w->cfn = fn;
    Err err = reg(cFi, w);
    if (!err) {
        m_wraps[cFi] = w;
    } else {
        delete w;
    }
    return err;
}

void QGoToC::unreg(int cFi)
{
    m_fns.remove(cFi);
    if (m_wraps.contains(cFi)) {
        FnWrap *w = m_wraps.take(cFi);
        delete w;
    }
}

void QGoToC::goCallRaw(int fi, const QByteArray &in_ba, OnGoCallRaw *out_cb)
{
    void *in_p = 0;
    int in_size = 0;
    copyData(in_ba, &in_p, &in_size);

    GoToC_Call_return r = GoToC_Call(fi, in_p, in_size);

    if (out_cb) {
        out_cb->onCall(static_cast<const char*>(r.r0), r.r1);
    }

    ::free(r.r0);
}

QByteArray QGoToC::goCall(int fi, const QByteArray &in_ba)
{
    void *in_p = 0;
    int in_size = 0;
    copyData(in_ba, &in_p, &in_size);

    GoToC_Call_return r = GoToC_Call(fi, in_p, in_size);

    QByteArray out_ba(static_cast<const char*>(r.r0), r.r1);

    ::free(r.r0);

    return out_ba;
}

void QGoToC::copyData(const QByteArray &in_ba, void** out_p, int *out_size)
{
    if (!in_ba.isEmpty()) {

        int ba_s = in_ba.size();
        void *ba_p = ::malloc(ba_s + 1u);
        ::memcpy(ba_p, in_ba.data(), ba_s + 1u);

        *out_p = ba_p;
        *out_size = ba_s;

    } else {
        *out_p = 0;
        *out_size = 0;
    }
}

void QGoToC::c_Call(int fi, void *in_p, int in_size, void **out_p, int *out_size)
{
    CFnRaw* fn = m_fns.value(fi, 0);
    //qDebug() << "qgotoc: c_Call fi: " << fi << ", ok:" << (fn != 0);
    if (fn) {
        QByteArray ba = fn->call(static_cast<const char*>(in_p), in_size);
        copyData(ba, out_p, out_size);
    }
}

void QGoToC::static_c_call(int i, void *in_p, int in_size, void **out_p, int *out_size)
{
    QGoToC::instance()->c_Call(i, in_p, in_size, out_p, out_size);
}

QByteArray QGoToC::FnWrap::call(const char *p, int s)
{
    if (this->cfn) {
        return this->cfn->call(QByteArray(p, s));
    }

    return QByteArray();
}

QByteArray QGoToC::OnEcho::call(const char *p, int s)
{
    QByteArray b(p, s);
    qDebug() << "qgotoc: onEcho in: " << b;
    b += " -> echo qt";
    return b;
}

void QGoToC::debug(bool is)
{
    char c = is ? 1 : 0;
    goCall(9, QByteArray(&c, 1));
}

void QGoToC::echo()
{
    echoGo();
    echoC();
}

void QGoToC::echoGo()
{
    QByteArray ba = goCall(10, QByteArray("hello from qt"));
    qDebug() << "qgotoc: echo ret: " << ba;
}

void QGoToC::echoC()
{
    goCall(11);
}

void QGoToC::benchGo(int bytes)
{
    QElapsedTimer timer;
    timer.start();
    QByteArray ba = goCall(20, QString::number(bytes).toLatin1());
    double elapsed = static_cast<double>(timer.nsecsElapsed()) / 1000000.0;
    qDebug() << "qgotoc: bench go elapsed: " << elapsed << "ms, bytes:" << ba.size();
}

void QGoToC::benchGoAll()
{
    benchGo(10);
    benchGo(100);
    benchGo(1000);
    benchGo(10000);
    benchGo(100000);
    benchGo(1000000);
    benchGo(10000000);
}
