local_qtwayland_server_protocol_treeland(libtreeland
    PROTOCOL ${CMAKE_CURRENT_SOURCE_DIR}/protocol/gxde-treeland-info-v1.xml
    BASENAME gxde-treeland-info-v1
)

impl_treeland(
    NAME
        module_gxdeinfo
    SOURCE
        ${CMAKE_SOURCE_DIR}/src/modules/gxde-info/gxdetreelandinfov1.h
        ${CMAKE_SOURCE_DIR}/src/modules/gxde-info/gxdetreelandinfov1.cpp
        ${CMAKE_BINARY_DIR}/src/modules/gxde-info/wayland-gxde-treeland-info-v1-server-protocol.c
    INCLUDE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
