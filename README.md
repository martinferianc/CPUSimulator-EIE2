# MIPS1-EIE2

This is repo containing the attempt to build a MIPS1 ISA Siumulator in C++

The simulator is built:

````
make simulator
````
and the resulting program is in `bin/test_mips`.

Example run and test after building:

````
make test
````

### Currently implemented features

| Instruction |  tests | passed | success |
|-------------|--------|--------|---------|
|         ADD |      5 |      5 |  100.0% |
|        ADDI |      3 |      3 |  100.0% |
|       ADDIU |      1 |      1 |  100.0% |
|        ADDU |     42 |     42 |  100.0% |
|         AND |      1 |      1 |  100.0% |
|        ANDI |      2 |      2 |  100.0% |
|         BEQ |      2 |      2 |  100.0% |
|        BGEZ |      3 |      3 |  100.0% |
|      BGEZAL |      3 |      3 |  100.0% |
|        BGTZ |      2 |      2 |  100.0% |
|        BLEZ |      3 |      3 |  100.0% |
|        BLTZ |      3 |      3 |  100.0% |
|      BLTZAL |      2 |      2 |  100.0% |
|         BNE |      2 |      2 |  100.0% |
|         DIV |      3 |      3 |  100.0% |
|        DIVU |      1 |      1 |  100.0% |
|           J |      1 |      1 |  100.0% |
|         JAL |      1 |      1 |  100.0% |
|        JALR |      1 |      1 |  100.0% |
|          JR |      1 |      1 |  100.0% |
|          LB |      2 |      2 |  100.0% |
|         LBU |      2 |      2 |  100.0% |
|          LH |      3 |      3 |  100.0% |
|         LHU |      2 |      2 |  100.0% |
|         LUI |      2 |      2 |  100.0% |
|          LW |      1 |      1 |  100.0% |
|         LWL |      2 |      2 |  100.0% |
|         LWR |      2 |      2 |  100.0% |
|        MFHI |      9 |      9 |  100.0% |
|        MFLO |      9 |      9 |  100.0% |
|        MTHI |      1 |      1 |  100.0% |
|        MTLO |      1 |      1 |  100.0% |
|        MULT |      1 |      1 |  100.0% |
|       MULTU |      3 |      3 |  100.0% |
|          OR |      2 |      2 |  100.0% |
|         ORI |      2 |      2 |  100.0% |
|          SB |      2 |      2 |  100.0% |
|          SH |      5 |      5 |  100.0% |
|         SLL |      2 |      2 |  100.0% |
|        SLLV |      1 |      1 |  100.0% |
|         SLT |      4 |      4 |  100.0% |
|        SLTI |      3 |      3 |  100.0% |
|       SLTIU |      4 |      4 |  100.0% |
|        SLTU |      2 |      2 |  100.0% |
|         SRA |      3 |      3 |  100.0% |
|        SRAV |      4 |      4 |  100.0% |
|         SRL |      2 |      2 |  100.0% |
|        SRLV |      2 |      2 |  100.0% |
|         SUB |      2 |      2 |  100.0% |
|        SUBU |      3 |      3 |  100.0% |
|          SW |      7 |      7 |  100.0% |
|         XOR |      3 |      3 |  100.0% |
|        XORI |      3 |      3 |  100.0% |
