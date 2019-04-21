cmake_minimum_required(VERSION 2.8)

project(qsr C)

set(VENDOR_PATH ${PROJECT_SOURCE_DIR}/ext)
set(VENDOR_SRC
	${VENDOR_PATH}/map/map.c
)

set(QSR_SRCS
	${PROJECT_SOURCE_DIR}/src/cparser.c
	${PROJECT_SOURCE_DIR}/src/ulog.c
	${PROJECT_SOURCE_DIR}/src/strutil.c
	${PROJECT_SOURCE_DIR}/src/main.c
	${VENDOR_SRC}
)

add_executable(qsr ${QSR_SRCS})
target_include_directories(qsr PRIVATE ${VENDOR_PATH}/map)
target_link_libraries(qsr procps)

install(TARGETS qsr DESTINATION "/usr/local/bin")
install(CODE "execute_process(COMMAND cp ${PROJECT_SOURCE_DIR}/qsr.service /etc/systemd/system)")
install(CODE "execute_process(COMMAND mkdir /usr/share/qsr)")
install(CODE "execute_process(COMMAND cp ${PROJECT_SOURCE_DIR}/qsr.conf /usr/share/qsr)")
install(CODE "execute_process(COMMAND systemctl enable qsr)")
install(CODE "execute_process(COMMAND systemctl start qsr)")
install(CODE "execute_process(COMMAND echo \"finished installing QSR\")")
