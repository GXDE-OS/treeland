#include "gxdetreelandinfov1.h"

#include "qwayland-server-gxde-treeland-info-v1.h"

#include <wserver.h>

#include <qwdisplay.h>

#define GXDE_TREELAND_INFO_V1_VERSION 1

#define GXDE_TREELAND_INFO_VENDOR "GXDE"
#define GXDE_TREELAND_INFO_BASE_VERSION "0.8.6"
#define GXDE_TREELAND_INFO_PATCH "gxde-wayland-adapt"

class GxdeTreelandInfoV1Private : public QtWaylandServer::gxde_treeland_info_v1
{
public:
    explicit GxdeTreelandInfoV1Private(GxdeTreelandInfoV1 *_q)
        : QtWaylandServer::gxde_treeland_info_v1()
        , q(_q)
    {
    }

    wl_global *global() const { return m_global; }

    GxdeTreelandInfoV1 *q;

protected:
    // 客户端绑定后立即发送 fork 标识信息。
    void bind_resource(Resource *resource) override
    {
        send_info(resource->handle,
                  QStringLiteral(GXDE_TREELAND_INFO_VENDOR),
                  QStringLiteral(GXDE_TREELAND_INFO_BASE_VERSION),
                  QStringLiteral(GXDE_TREELAND_INFO_PATCH));
    }

    void destroy(Resource *resource) override
    {
        wl_resource_destroy(resource->handle);
    }
};

GxdeTreelandInfoV1::GxdeTreelandInfoV1(QObject *parent)
    : QObject(parent)
    , d(new GxdeTreelandInfoV1Private(this))
{
}

GxdeTreelandInfoV1::~GxdeTreelandInfoV1() = default;

void GxdeTreelandInfoV1::create(WServer *server)
{
    d->init(server->handle()->handle(), GXDE_TREELAND_INFO_V1_VERSION);
}

void GxdeTreelandInfoV1::destroy([[maybe_unused]] WServer *server)
{
    d->globalRemove();
}

wl_global *GxdeTreelandInfoV1::global() const
{
    return d->global();
}

QByteArrayView GxdeTreelandInfoV1::interfaceName() const
{
    return d->interfaceName();
}
