add_executable(test_persistence
    test_serialization.cpp
    test_deserialization.cpp
    test_serializer_contract.cpp
)

target_link_libraries(test_persistence PRIVATE persistence)
