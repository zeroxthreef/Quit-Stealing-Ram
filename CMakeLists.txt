cmake_minimum_required(VERSION 2.8)

project(QSR)

add_executable(QSR "${PROJECT_SOURCE_DIR}/main.c")

target_link_libraries(QSR procps)

install(TARGETS QSR DESTINATION "/usr/local/bin")
install(CODE "execute_process(COMMAND cp ${PROJECT_SOURCE_DIR}/QSR.service /etc/systemd/system)")
install(CODE "execute_process(COMMAND systemctl enable QSR)")
install(CODE "execute_process(COMMAND systemctl start QSR)")
install(CODE "execute_process(COMMAND echo \"finished installing QSR\")")