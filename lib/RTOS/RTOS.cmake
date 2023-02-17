set(RTOS_INCLUDE_DIR
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/include
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Demo/Common/include
)

set(RTOS_SOURCES
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/event_groups.c
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/stream_buffer.c
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/timers.c
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/list.c
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/queue.c
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/tasks.c
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/portable/MemMang/heap_3.c
)

if(UNIX)
    set(RTOS_INCLUDE_DIR
        ${RTOS_INCLUDE_DIR}
        ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix
        ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix/utils
    )

    set(RTOS_SOURCES
        ${RTOS_SOURCES}
        ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix/port.c
        ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c
    )
elseif(WIN32)
    set(RTOS_INCLUDE_DIR
        ${RTOS_INCLUDE_DIR}
        ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/portable/MSVC-MingW
    )

    set(RTOS_SOURCES
        ${RTOS_SOURCES}
        ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/portable/MSVC-MingW/port.c
    )
endif()
