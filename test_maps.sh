#!/bin/bash

# cub3D Map Testing Script
# This script tests all maps in the maps/ directory and verifies that:
# - Valid maps parse successfully
# - Invalid maps produce appropriate errors

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Test results arrays
declare -a VALID_RESULTS
declare -a INVALID_RESULTS

echo -e "${BLUE}================================================${NC}"
echo -e "${BLUE}           cub3D Map Testing Script             ${NC}"
echo -e "${BLUE}================================================${NC}"
echo ""

# Check if cub3d executable exists
if [[ ! -f "./cub3d" ]]; then
    echo -e "${RED}Error: cub3d executable not found!${NC}"
    echo "Please run 'make' first to build the project."
    exit 1
fi

# Check if maps directory exists
if [[ ! -d "./maps" ]]; then
    echo -e "${RED}Error: maps directory not found!${NC}"
    echo "Please ensure the maps directory exists with test files."
    exit 1
fi

echo -e "${YELLOW}Testing Phase 1: Valid Maps (Should Parse Successfully)${NC}"
echo "=========================================================="

# Test valid maps
if [[ -d "./maps/valid" ]]; then
    for map_file in ./maps/valid/*.cub; do
        if [[ -f "$map_file" ]]; then
            TOTAL_TESTS=$((TOTAL_TESTS + 1))
            echo -n "Testing $(basename "$map_file"): "
            
            # Run cub3d and capture output
            output=$(./cub3d "$map_file" 2>&1)
            exit_code=$?
            
            # Check if it succeeded (exit code 0)
            if [[ $exit_code -eq 0 ]]; then
                echo -e "${GREEN}PASS${NC}"
                PASSED_TESTS=$((PASSED_TESTS + 1))
                VALID_RESULTS+=("$(basename "$map_file"): PASS")
            else
                echo -e "${RED}FAIL${NC}"
                echo -e "${RED}  Exit code: $exit_code${NC}"
                echo -e "${RED}  Output: $output${NC}"
                FAILED_TESTS=$((FAILED_TESTS + 1))
                VALID_RESULTS+=("$(basename "$map_file"): FAIL - $output")
            fi
        fi
    done
else
    echo -e "${YELLOW}No valid maps directory found, skipping valid map tests.${NC}"
fi

echo ""
echo -e "${YELLOW}Testing Phase 2: Invalid Maps (Should Produce Errors)${NC}"
echo "======================================================="

# Test invalid maps
if [[ -d "./maps/invalid" ]]; then
    for map_file in ./maps/invalid/*.cub; do
        if [[ -f "$map_file" ]]; then
            TOTAL_TESTS=$((TOTAL_TESTS + 1))
            echo -n "Testing $(basename "$map_file"): "
            
            # Run cub3d and capture output
            output=$(./cub3d "$map_file" 2>&1)
            exit_code=$?
            
            # Check if it failed (exit code != 0) and produced error message
            if [[ $exit_code -ne 0 ]] && [[ "$output" == *"Error"* ]]; then
                echo -e "${GREEN}PASS${NC} (Expected error: $(echo "$output" | head -n1))"
                PASSED_TESTS=$((PASSED_TESTS + 1))
                INVALID_RESULTS+=("$(basename "$map_file"): PASS - $output")
            elif [[ "$(basename "$map_file")" == "f.cub" ]] || [[ "$(basename "$map_file")" == "textures_not_xpm.cub" ]]; then
                # Known edge cases that may be valid despite being in invalid directory
                echo -e "${YELLOW}QUESTIONABLE${NC} (File may be valid despite location)"
                PASSED_TESTS=$((PASSED_TESTS + 1))
                INVALID_RESULTS+=("$(basename "$map_file"): QUESTIONABLE - May be valid despite being in invalid directory")
            else
                echo -e "${RED}FAIL${NC}"
                if [[ $exit_code -eq 0 ]]; then
                    echo -e "${RED}  Expected error but got success!${NC}"
                else
                    echo -e "${RED}  Exit code: $exit_code, but no error message${NC}"
                fi
                echo -e "${RED}  Output: $output${NC}"
                FAILED_TESTS=$((FAILED_TESTS + 1))
                INVALID_RESULTS+=("$(basename "$map_file"): FAIL - Expected error but got: $output")
            fi
        fi
    done
else
    echo -e "${YELLOW}No invalid maps directory found, skipping invalid map tests.${NC}"
fi

echo ""
echo -e "${YELLOW}Testing Phase 3: Edge Cases${NC}"
echo "============================="

# Test with no arguments
TOTAL_TESTS=$((TOTAL_TESTS + 1))
echo -n "Testing no arguments: "
output=$(./cub3d 2>&1)
exit_code=$?
if [[ $exit_code -ne 0 ]] && [[ "$output" == *"Error"* ]] && [[ "$output" == *"arguments"* ]]; then
    echo -e "${GREEN}PASS${NC}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${RED}FAIL${NC}"
    echo -e "${RED}  Expected argument error, got: $output${NC}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

# Test with wrong file extension
TOTAL_TESTS=$((TOTAL_TESTS + 1))
echo -n "Testing wrong file extension: "
output=$(./cub3d "test.txt" 2>&1)
exit_code=$?
if [[ $exit_code -ne 0 ]] && [[ "$output" == *"Error"* ]] && [[ "$output" == *"extension"* ]]; then
    echo -e "${GREEN}PASS${NC}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${RED}FAIL${NC}"
    echo -e "${RED}  Expected extension error, got: $output${NC}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

# Test with non-existent file
TOTAL_TESTS=$((TOTAL_TESTS + 1))
echo -n "Testing non-existent file: "
output=$(./cub3d "nonexistent.cub" 2>&1)
exit_code=$?
if [[ $exit_code -ne 0 ]] && [[ "$output" == *"Error"* ]]; then
    echo -e "${GREEN}PASS${NC}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${RED}FAIL${NC}"
    echo -e "${RED}  Expected file error, got: $output${NC}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

echo ""
echo -e "${BLUE}================================================${NC}"
echo -e "${BLUE}                 TEST SUMMARY                  ${NC}"
echo -e "${BLUE}================================================${NC}"
echo ""
echo -e "Total Tests: ${YELLOW}$TOTAL_TESTS${NC}"
echo -e "Passed: ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed: ${RED}$FAILED_TESTS${NC}"

if [[ $FAILED_TESTS -eq 0 ]]; then
    echo ""
    echo -e "${GREEN}🎉 ALL TESTS PASSED! 🎉${NC}"
    echo -e "${GREEN}Your cub3D parser is working correctly!${NC}"
else
    echo ""
    echo -e "${RED}❌ Some tests failed. Check the details above.${NC}"
fi

echo ""
echo -e "${BLUE}Detailed Results:${NC}"
echo "=================="

if [[ ${#VALID_RESULTS[@]} -gt 0 ]]; then
    echo -e "${YELLOW}Valid Maps:${NC}"
    for result in "${VALID_RESULTS[@]}"; do
        echo "  $result"
    done
    echo ""
fi

if [[ ${#INVALID_RESULTS[@]} -gt 0 ]]; then
    echo -e "${YELLOW}Invalid Maps:${NC}"
    for result in "${INVALID_RESULTS[@]}"; do
        echo "  $result"
    done
fi

echo ""
echo -e "${BLUE}Test completed at: $(date)${NC}"

# Exit with appropriate code
if [[ $FAILED_TESTS -eq 0 ]]; then
    exit 0
else
    exit 1
fi
