cmake_minimum_required(VERSION 3.10)
project(ScheduleManagementSystem)

include(ExternalProject)

ExternalProject_Add(
    jansson
    PREFIX ${CMAKE_BINARY_DIR}/jansson
    GIT_REPOSITORY https://github.com/akheron/jansson
    GIT_TAG master  # Güncel sürüm etiketi
    UPDATE_DISCONNECTED 1
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=D:/Program Files
)

# Jansson'ı Ekleyin
ExternalProject_Get_Property(jansson install_dir)
set(JANSSON_INCLUDE_DIRS ${install_dir}/include PARENT_SCOPE)
set(JANSSON_LIBRARY_DIRS ${install_dir}/lib PARENT_SCOPE)

# Projenizin Kaynak Dosyalarını Ekleyin
add_executable(ScheduleManagementSystem ScheduleManagementSystem.c)

# Projenizin bağımlılıklarını Ayarlayın
add_dependencies(ScheduleManagementSystem jansson)

# Jansson başlık dosyasını ve kütüphane dosyasını projeye ekleyin
target_include_directories(ScheduleManagementSystem PRIVATE ${install_dir}/include)
#target_link_libraries(ScheduleManagementSystem PRIVATE debug $<$<CONFIG:Debug>:${install_dir}/lib/jansson_d.lib> optimized $<$<NOT:$<CONFIG:Debug>>:${install_dir}/lib/jansson.lib>)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(ScheduleManagementSystem PRIVATE ${install_dir}/lib/jansson_d.lib)
else()
    target_link_libraries(ScheduleManagementSystem PRIVATE ${install_dir}/lib/jansson.lib)
endif()


#target_link_libraries(ScheduleManagementSystem PRIVATE ${install_dir}/lib/jansson.lib)

# Yürütülebilir dosyanın çıktı dizinini belirtin (örnek olarak bin dizini)
set_target_properties(ScheduleManagementSystem PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
