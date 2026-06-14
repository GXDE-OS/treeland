#pragma once

#include <wserver.h>

#include <QObject>

#include <memory>

WAYLIB_SERVER_USE_NAMESPACE

class GxdeTreelandInfoV1Private;

class GxdeTreelandInfoV1 : public QObject, public WServerInterface
{
    Q_OBJECT
public:
    explicit GxdeTreelandInfoV1(QObject *parent = nullptr);
    ~GxdeTreelandInfoV1() override;

    QByteArrayView interfaceName() const override;

protected:
    void create(WServer *server) override;
    void destroy(WServer *server) override;
    wl_global *global() const override;

private:
    std::unique_ptr<GxdeTreelandInfoV1Private> d;
};
