#ifndef GOTOCBRIDGE_H
#define GOTOCBRIDGE_H

#include <QByteArray>
#include <QHash>

class QGoToC
{
public:

    static QGoToC* instance() {
        static QGoToC b;
        return &b;
    }

    struct Err {
        QString text;
        Err() {}
        Err(const QString &t) : text(t) {}
        bool toBool() const { return !text.isEmpty(); }
        inline operator bool() const { return toBool(); }
        inline bool operator!() const { return !toBool(); }
    };

    struct CFnRaw {
        virtual QByteArray call(const char *p, int s) = 0;
    };

    struct CFn {
        virtual ~CFn(){}
        virtual QByteArray call(const QByteArray &in_ba) = 0;
    };

    Err reg(int cFi, CFnRaw *fn);
    Err reg(int cFi, CFn *fn);
    void unreg(int cFi);

    struct OnGoCallRaw {
        virtual void onCall(const char *p, int s) = 0;
    };

    void goCallRaw(int fi, const QByteArray &in_ba, OnGoCallRaw *out_cb);
    QByteArray goCall(int fi, const QByteArray &in_ba = QByteArray());


    void debug(bool is);

    void echo();
    void echoGo();
    void echoC();

    void benchGo(int bytes);
    void benchGoAll();

private:

    QGoToC();
    ~QGoToC();

    struct OnEcho : public CFnRaw {
        virtual ~OnEcho() {}
        QByteArray call(const char *p, int s);
    };

    struct FnWrap : public CFnRaw {
        CFn *cfn;
        FnWrap() : cfn(0){}
        virtual ~FnWrap() {}
        QByteArray call(const char *p, int s);
    };

    static void static_c_call(int i, void* in_p, int in_size, void** out_p, int* out_size);

    void c_Call(int fi, void* in_p, int in_size, void** out_p, int* out_size);

    void copyData(const QByteArray &in_ba, void** out_p, int *out_size);

    OnEcho m_onEcho;

    QHash<int, CFnRaw*> m_fns;
    QHash<int, FnWrap*> m_wraps;
};

#endif // GOTOCBRIDGE_H
