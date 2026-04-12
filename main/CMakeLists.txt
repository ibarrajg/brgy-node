idf_component_register(
    SRCS 
        "main.cpp"
        "get_message.cpp"
        "message_frame.cpp"
        "frame_parser.cpp"
        "crc.cpp"  
        "lora_uart.cpp"
        "reception.cpp"
        "transmission.cpp"
        "node_logic.cpp"
        "loop_guard.cpp"
    INCLUDE_DIRS "."
    )