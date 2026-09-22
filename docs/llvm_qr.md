# llvm quick reference

## naming pattern:

| pattern | meaning | example |
| --- | --- | --- | 
| `LLVM*` | lib prefix no fancy here | `LLVMBuildAdd`|
| `LLVMBuild*` | it'll emit ir intruction | `LLVMBuildRet` |
| `LLVMConst*` | compile time constants | `LLVMConstInt` |
| `LLVMGet*` | read/query something, no mutation | `LLVMGetInsertBlock` |
| `LLVMSet*` | mutate a property | `LLVMSetValueName` |
| `LLVMAdd*` | add something to a container(module, not block) | `LLVMAddFunction` |
| `LLVMAppend*` | append to ordered list | `LLVMAppendBasicBlock` |
| `LLVMCreate*` | allocate + init a new object | `LLVMCreateBuilder` |
| `LLVMDispose*` | freeee the memoryyy | `LLVMDisposeBuilder` |
| `LLVMInt*Type` | get type object | `LLVMInt32Type` |
| `LLVM*Ref` | reference to opque llvm object (can't deref) | `LLVMValueRef` |

## llvm context
a memory arena that owns all llvm object e.g. types, values, blocks

```
    llvm context ref
        |
        |--> types (i32, i64, void ...)
        |
        |--> modules
        |        |--> module ref
        |                |
        |                |--> func
        |
        |--> all ir objects
```

## bare vs in context

`LLVMInt32Type()`  -> global context
`LLVMInt32TypeInContext()`  -> explicit ownership

## function/basic block/ instruction

```
Module 
    |--> function (e.g main)
            |
            |--> BasicBlock (e.g. entry )
            |       |--> instruction
            |       |--> instruction
            |       |--> instruction
            |
            |--> BasicBlock (e.g. loop )
            |       |--> ...
            |
            |--> BasicBlock (e.g. exit )
                    |--> ...
```


## builder
builder -> cursor
whenever any build called, it'll do emit ir at that cursor position of that block

- `LLVMPositionBuilderAtEnd(builder, block)` -> move cursor to end of block.
- `LLVMGetInsertBlock(builder)` -> which block cursor currently in
- `LLVMGetBasicBlockTerminator` -> is the block the builder currently in already terminated?

## terminator 
every block must ends with terminator, without it, ir is invalid
it says, where to go next. 
it can be `ret`(return), `br` (branch), `switch`, `unreachable`

`br` -> jump to another block

# TODO: continue learning, then about the how llvm work with memory, then implemnent the var
