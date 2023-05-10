# Copyright 2019-2023 Cambridge Quantum Computing
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.23)
project(tkwsm CXX)

find_package(tkassert CONFIG REQUIRED)
find_package(tkrng CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS yes)
endif()

if(APPLE)
    set(CMAKE_INSTALL_NAME_DIR "@loader_path")
    set(CMAKE_BUILD_WITH_INSTALL_NAME_DIR ON)
endif()

add_library(tkwsm)

if(MSVC)
  target_compile_options(tkwsm PRIVATE /W4 /WX  /wd4267)
else()
  target_compile_options(tkwsm PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

set(PROFILE_COVERAGE no CACHE BOOL "Build library with profiling for test coverage")
IF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    IF (PROFILE_COVERAGE)
        target_compile_options(tkwsm PRIVATE -g --coverage)
        target_link_options(tkwsm PUBLIC --coverage)
        # Bug in gcc 10: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=95353
        IF (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11)
            target_compile_options(tkwsm PRIVATE -Wno-stringop-overflow)
        ENDIF()
    ENDIF()
ENDIF()

target_include_directories(tkwsm PUBLIC include/tkwsm)
target_link_libraries(tkwsm PRIVATE tkassert::tkassert)
target_link_libraries(tkwsm PRIVATE tkrng::tkrng)
target_link_libraries(tkwsm PRIVATE boost::boost)

target_sources(tkwsm
    PRIVATE
        src/Common/BitFunctions.cpp
        src/Common/DyadicFraction.cpp
        src/EndToEndWrappers/MainSolver.cpp
        src/EndToEndWrappers/MainSolverParameters.cpp
        src/EndToEndWrappers/PreSearchComponents.cpp
        src/EndToEndWrappers/SolutionWSM.cpp
        src/GraphTheoretic/DerivedGraphs.cpp
        src/GraphTheoretic/DerivedGraphsCalculator.cpp
        src/GraphTheoretic/DomainInitialiser.cpp
        src/GraphTheoretic/FilterUtils.cpp
        src/GraphTheoretic/GeneralStructs.cpp
        src/GraphTheoretic/NearNeighboursData.cpp
        src/GraphTheoretic/NeighboursData.cpp
        src/GraphTheoretic/VertexRelabelling.cpp
        src/InitPlacement/EndToEndIQP.cpp
        src/InitPlacement/FastRandomBits.cpp
        src/InitPlacement/InputStructs.cpp
        src/InitPlacement/MonteCarloCompleteTargetSolution.cpp
        src/InitPlacement/MonteCarloManager.cpp
        src/InitPlacement/PrunedTargetEdges.cpp
        src/InitPlacement/SolutionJumper.cpp
        src/InitPlacement/UtilsIQP.cpp
        src/Reducing/DerivedGraphsReducer.cpp
        src/Reducing/DistancesReducer.cpp
        src/Reducing/HallSetReduction.cpp
        src/Reducing/ReducerWrapper.cpp
        src/Searching/DomainsAccessor.cpp
        src/Searching/NodeListTraversal.cpp
        src/Searching/NodesRawData.cpp
        src/Searching/SearchBranch.cpp
        src/Searching/ValueOrdering.cpp
        src/Searching/VariableOrdering.cpp
        src/Searching/WeightCalculator.cpp
        src/WeightPruning/WeightChecker.cpp
        src/WeightPruning/WeightNogoodDetector.cpp
        src/WeightPruning/WeightNogoodDetectorManager.cpp
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES
        include/tkwsm/Common/BitFunctions.hpp
        include/tkwsm/Common/DyadicFraction.hpp
        include/tkwsm/Common/GeneralUtils.hpp
        include/tkwsm/Common/LogicalStack.hpp
        include/tkwsm/Common/ReusableStorage.hpp
        include/tkwsm/Common/SpecialExceptions.hpp
        include/tkwsm/Common/TemporaryRefactorCode.hpp
        include/tkwsm/EndToEndWrappers/MainSolver.hpp
        include/tkwsm/EndToEndWrappers/MainSolverParameters.hpp
        include/tkwsm/EndToEndWrappers/PreSearchComponents.hpp
        include/tkwsm/EndToEndWrappers/SearchComponents.hpp
        include/tkwsm/EndToEndWrappers/SolutionData.hpp
        include/tkwsm/EndToEndWrappers/SolutionWSM.hpp
        include/tkwsm/GraphTheoretic/DerivedGraphsCalculator.hpp
        include/tkwsm/GraphTheoretic/DerivedGraphs.hpp
        include/tkwsm/GraphTheoretic/DerivedGraphStructs.hpp
        include/tkwsm/GraphTheoretic/DomainInitialiser.hpp
        include/tkwsm/GraphTheoretic/FilterUtils.hpp
        include/tkwsm/GraphTheoretic/GeneralStructs.hpp
        include/tkwsm/GraphTheoretic/NearNeighboursData.hpp
        include/tkwsm/GraphTheoretic/NeighboursData.hpp
        include/tkwsm/GraphTheoretic/VertexRelabelling.hpp
        include/tkwsm/InitPlacement/EndToEndIQP.hpp
        include/tkwsm/InitPlacement/FastRandomBits.hpp
        include/tkwsm/InitPlacement/InputStructs.hpp
        include/tkwsm/InitPlacement/MonteCarloCompleteTargetSolution.hpp
        include/tkwsm/InitPlacement/MonteCarloManager.hpp
        include/tkwsm/InitPlacement/PrunedTargetEdges.hpp
        include/tkwsm/InitPlacement/SolutionJumper.hpp
        include/tkwsm/InitPlacement/UtilsIQP.hpp
        include/tkwsm/Reducing/DerivedGraphsReducer.hpp
        include/tkwsm/Reducing/DistancesReducer.hpp
        include/tkwsm/Reducing/HallSetReduction.hpp
        include/tkwsm/Reducing/ReducerWrapper.hpp
        include/tkwsm/Searching/DomainsAccessor.hpp
        include/tkwsm/Searching/NodeListTraversal.hpp
        include/tkwsm/Searching/NodesRawData.hpp
        include/tkwsm/Searching/SearchBranch.hpp
        include/tkwsm/Searching/ValueOrdering.hpp
        include/tkwsm/Searching/VariableOrdering.hpp
        include/tkwsm/Searching/WeightCalculator.hpp
        include/tkwsm/WeightPruning/WeightChecker.hpp
        include/tkwsm/WeightPruning/WeightNogoodDetector.hpp
        include/tkwsm/WeightPruning/WeightNogoodDetectorManager.hpp
        )

install(TARGETS tkwsm FILE_SET HEADERS)

if(MSVC)
    install(TARGETS tkwsm
        RUNTIME DESTINATION bin
        RUNTIME DESTINATION lib
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)
endif()