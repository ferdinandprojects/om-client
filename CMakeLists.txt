cmake_minimum_required(VERSION 3.1)

project(om-client)

set(SRC_DIR src)
set(OM_SRC 
    ${SRC_DIR}/main.c
    ${SRC_DIR}/om_client.c
    ${SRC_DIR}/om_client_connection.c
)

add_library(omclient STATIC ${OM_SRC})

add_executable(main ${OM_SRC})

