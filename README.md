# MIPS I CPU Simulator
This is [MIPS I](https://en.wikipedia.org/wiki/MIPS_architecture) CPU simulator and a testing library.
The simulator is almost a complete MIPS I CPU: all but floating-point and coprocessor instructions have been implemented. Exceptions were implemented using predefined exception return codes.

## Specification
This piece of work was done in partial fulfilment of an advanced computer architecture course (EE2-13). The specification for the coursework can be found [here](https://github.com/m8pple/arch2-2016-cw).

## Currently implemented features and tests

| Instruction |  tests | passed |
|-------------|--------|--------|
|         ADD |      5 |      5 |
|        ADDI |      3 |      3 |
|       ADDIU |      1 |      1 |
|        ADDU |     42 |     42 |
|         AND |      1 |      1 |
|        ANDI |      2 |      2 |
|         BEQ |      2 |      2 |
|        BGEZ |      3 |      3 |
|      BGEZAL |      3 |      3 |
|        BGTZ |      2 |      2 |
|        BLEZ |      3 |      3 |
|        BLTZ |      3 |      3 |
|      BLTZAL |      2 |      2 |
|         BNE |      2 |      2 |
|         DIV |      3 |      3 |
|        DIVU |      1 |      1 |
|           J |      1 |      1 |
|         JAL |      1 |      1 |
|        JALR |      1 |      1 |
|          JR |      1 |      1 |
|          LB |      2 |      2 |
|         LBU |      2 |      2 |
|          LH |      3 |      3 |
|         LHU |      2 |      2 |
|         LUI |      2 |      2 |
|          LW |      1 |      1 |
|         LWL |      2 |      2 |
|         LWR |      2 |      2 |
|        MFHI |      9 |      9 |
|        MFLO |      9 |      9 |
|        MTHI |      1 |      1 |
|        MTLO |      1 |      1 |
|        MULT |      1 |      1 |
|       MULTU |      3 |      3 |
|          OR |      2 |      2 |
|         ORI |      2 |      2 |
|          SB |      2 |      2 |
|          SH |      5 |      5 |
|         SLL |      2 |      2 |
|        SLLV |      1 |      1 |
|         SLT |      4 |      4 |
|        SLTI |      3 |      3 |
|       SLTIU |      4 |      4 |
|        SLTU |      2 |      2 |
|         SRA |      3 |      3 |
|        SRAV |      4 |      4 |
|         SRL |      2 |      2 |
|        SRLV |      2 |      2 |
|         SUB |      2 |      2 |
|        SUBU |      3 |      3 |
|          SW |      7 |      7 |
|         XOR |      3 |      3 |
|        XORI |      3 |      3 |

## Building and Running
The simulator is built:
```
make simulator
```
and the resulting program is in `bin/test_mips`.

Example build and then test:

```
make test
```

## Credits
- The code in [fragments](/fragments) was provided by the course lecturer, [David Thomas](https://github.com/m8pple), for use of automatic marking, and so it is not my own.
- The header files in [include](/include) were also provided by [David Thomas](https://github.com/m8pple), these define the API for automatic marking and testing.
- All other code was written by Martin Ferianc in 2016.

