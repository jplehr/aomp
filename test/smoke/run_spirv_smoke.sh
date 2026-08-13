#!/bin/bash
#
# SPIR-V Smoke Test Runner with Result Capture
# Captures compile and runtime output, summarizes pass/fail status
#
# Usage: ./run_spirv_smoke.sh [JIT_OPT_LEVEL]
#   JIT_OPT_LEVEL: Optional, e.g., "-O2" to set AMD_COMGR_DRIVER_OPTIONS_APPEND
#
# Output files:
#   /tmp/spirv_smoke_<timestamp>.log         - Complete test output
#   /tmp/spirv_smoke_<timestamp>_summary.txt - Pass/fail summary
#

set -o pipefail

# Configuration
AOMP="${AOMP:-/home/janplehr/rocm/aomp_24.0-spirv-in-openmp/lib/llvm}"
AOMP_GPU="${AOMP_GPU:-amdgcnspirv}"
JIT_OPT="${1:-}"  # Optional: "-O2", "-O3", etc.

# Disable the COMGR JIT cache by default: after a compiler or COMGR change a
# warm cache silently returns stale JIT results. Override by exporting
# AMD_COMGR_CACHE=1 when a cached run is explicitly wanted.
AMD_COMGR_CACHE="${AMD_COMGR_CACHE:-0}"

# Output files
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
FULL_LOG="/tmp/spirv_smoke_${TIMESTAMP}.log"
SUMMARY="/tmp/spirv_smoke_${TIMESTAMP}_summary.txt"

# Colors
RED='\033[0;31m'
GRN='\033[0;32m'
YEL='\033[0;33m'
BLU='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLU}=== SPIR-V Smoke Test Runner ===${NC}"
echo "AOMP: $AOMP"
echo "AOMP_GPU: $AOMP_GPU"
echo "JIT Optimization: ${JIT_OPT:-'(default/none)'}"
echo "AMD_COMGR_CACHE: $AMD_COMGR_CACHE"
echo "Full log: $FULL_LOG"
echo "Summary: $SUMMARY"
echo ""

# Set up environment
export AOMP
export AOMP_GPU
export LIBOMPTARGET_AMDGPU_LLD_PATH="$AOMP/bin/ld.lld"
export OMP_TARGET_OFFLOAD=MANDATORY
export AMD_COMGR_CACHE

if [ -n "$JIT_OPT" ]; then
    export AMD_COMGR_DRIVER_OPTIONS_APPEND="$JIT_OPT"
    echo "AMD_COMGR_DRIVER_OPTIONS_APPEND=$JIT_OPT"
fi

# Change to smoke test directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR" || exit 1

# Header for summary
{
    echo "=== SPIR-V Smoke Test Summary ==="
    echo "Date: $(date)"
    echo "AOMP: $AOMP"
    echo "AOMP_GPU: $AOMP_GPU"
    echo "JIT Optimization: ${JIT_OPT:-'(default/none)'}"
    echo "AMD_COMGR_CACHE: $AMD_COMGR_CACHE"
    echo ""
} > "$SUMMARY"

# Run the smoke tests, capturing all output
echo -e "${YEL}Running smoke tests... (this will take a while)${NC}"
echo ""

./check_smoke.sh 2>&1 | tee "$FULL_LOG"

echo ""
echo -e "${BLU}=== Parsing Results ===${NC}"

# Parse results directly from log file
# check_smoke.sh outputs:
#   - Compile failures: "testname: Make Failed"
#   - Runtime results: "testname return code: N" (0=pass, non-zero=fail)

# Extract compile failures (lines containing ": Make Failed")
grep ': Make Failed$' "$FULL_LOG" | sed 's/: Make Failed$//' | sort -u > /tmp/compile_fails_$$.txt
COMPILE_FAIL_COUNT=$(wc -l < /tmp/compile_fails_$$.txt)

# Extract runtime failures (return code non-zero)
# Pattern: "testname return code: N" where N > 0
grep -E ' return code: [1-9][0-9]*$' "$FULL_LOG" | awk '{print $1}' | sort -u > /tmp/runtime_fails_$$.txt
RUNTIME_FAIL_COUNT=$(wc -l < /tmp/runtime_fails_$$.txt)

# Extract passing tests (return code 0)
grep -E ' return code: 0$' "$FULL_LOG" | awk '{print $1}' | sort -u > /tmp/passing_$$.txt
PASS_COUNT=$(wc -l < /tmp/passing_$$.txt)

TOTAL=$((PASS_COUNT + RUNTIME_FAIL_COUNT + COMPILE_FAIL_COUNT))

# Categorize compile failures
FORTRAN_FAILS=$(grep -c '^flang' /tmp/compile_fails_$$.txt 2>/dev/null || echo 0)
OTHER_COMPILE_FAILS=$((COMPILE_FAIL_COUNT - FORTRAN_FAILS))

# Print to console and summary file
{
    echo "----------------------------------------"
    echo "TOTAL TESTS: $TOTAL"
    echo ""
    echo -e "${GRN}PASSED: $PASS_COUNT${NC}"
    echo -e "${RED}RUNTIME FAILURES: $RUNTIME_FAIL_COUNT${NC}"
    echo -e "${RED}COMPILE FAILURES: $COMPILE_FAIL_COUNT${NC}"
    echo "  - Fortran (expected): $FORTRAN_FAILS"
    echo "  - Other: $OTHER_COMPILE_FAILS"
    echo "----------------------------------------"
} | tee -a "$SUMMARY"

# Add detailed lists to summary
{
    echo ""
    echo "=== PASSING TESTS ($PASS_COUNT) ==="
    if [ -s /tmp/passing_$$.txt ]; then
        cat /tmp/passing_$$.txt
    else
        echo "(none)"
    fi

    echo ""
    echo "=== RUNTIME FAILURES ($RUNTIME_FAIL_COUNT) ==="
    if [ -s /tmp/runtime_fails_$$.txt ]; then
        cat /tmp/runtime_fails_$$.txt
    else
        echo "(none)"
    fi

    echo ""
    echo "=== COMPILE FAILURES ($COMPILE_FAIL_COUNT) ==="
    echo "--- Other ($OTHER_COMPILE_FAILS) ---"
    grep -v '^flang' /tmp/compile_fails_$$.txt 2>/dev/null || echo "(none)"
    echo ""
    echo "--- Fortran ($FORTRAN_FAILS) - expected, no SPIR-V support ---"
    grep '^flang' /tmp/compile_fails_$$.txt 2>/dev/null || echo "(none)"
} >> "$SUMMARY"

# Cleanup temp files
rm -f /tmp/compile_fails_$$.txt /tmp/runtime_fails_$$.txt /tmp/passing_$$.txt

echo ""
echo -e "${BLU}Summary written to: $SUMMARY${NC}"
echo -e "${BLU}Full log written to: $FULL_LOG${NC}"

# Create symlinks to latest
ln -sf "$FULL_LOG" /tmp/spirv_smoke_latest.log
ln -sf "$SUMMARY" /tmp/spirv_smoke_latest_summary.txt

# Exit with appropriate code
if [ "$RUNTIME_FAIL_COUNT" -gt 0 ] || [ "$OTHER_COMPILE_FAILS" -gt 0 ]; then
    exit 1
else
    exit 0
fi
