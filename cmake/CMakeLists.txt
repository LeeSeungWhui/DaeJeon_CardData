#최소 버젼
cmake_minimum_required(VERSION 2.8)

#프로젝트 이름
project(Daejeon_Raw)

# 컴파일플래그
set(CMAKE_CXX_FLAGS "-std=c++0x")

#컴파일러
set(CMAKE_C_COMPILER "g++")

# 인클루드 디렉토리
INCLUDE_DIRECTORIES(/usr/include)

# 라이브러리
LINK_LIBRARIES(mysqlcppconn-static mysqlclient)

# 라이브러리 디렉토리
LINK_DIRECTORIES(/usr/lib)

# 컴파일 옵션
ADD_COMPILE_OPTIONS(-g -Wall)

# 소스파일
set(SOURCE_FILES ../src/Main.cpp ../src/Raw/Raw.cpp ../src/Raw/RawData.cpp ../src/IntWrapper/IntWrapper.cpp ../src/DbConnection/DbConnection.cpp ../src/Bis/Bis.cpp ../src/Bis/BisData.cpp ../src/Config/ConfigFile.cpp)

# 출력 디렉토리
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

# 바이너리
add_executable(Daejeon_Raw ${SOURCE_FILES})

# 라이브러리 연결
target_link_libraries(Daejeon_Raw dl)
target_link_libraries(Daejeon_Raw pthread)
target_link_libraries(Daejeon_Raw mysqlclient)
target_link_libraries(Daejeon_Raw mysqlcppconn-static)

# Install 디렉토리
install(FILES ../bin/Daejeon_Raw DESTINATION ~/Daejeon_Raw)
