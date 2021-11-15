* Conversion Flow
the whole applyFullConversion is like

1. collect ops to be converted

   the operations are collected from outside to inside, and from top
   to bottom

2. for each ops to be converted, call OperationConverter::convert()

   the whole action is to use a legalizer to legalize the op, so the
   core action is OperationLegalizer::legalize, which include:

   1. print debug log

   2. test if the op is already legal

      if it's already legal, the job is done; if it's recursively
      legal, mark its children ignored

   3. legalize an illegal op
   
      at first, try to fold it, call
      OperationLegalizer::legalizeWithFold inside it calls
      OpBuilder::tryFold
	 
       1. use a constant matcher(inside it checks if the op has
          ConstantLike trait) to match the op if the op is already a
          constant, the job is done
		
	   2. ??? check if the operands can be folded ???
	   
	   3. call Operation::fold, the folded operands are supplied as
          attribute array inside; firstly it calls foldHook registered
          in the abstract operation then it calls the interface method
          fold, if the op implements DialectFoldInterface
		
       the second choice is to try to do a pattern match & rewrite,
       via OperationLegalizer::legalizeWithPattern; inside it calls
       PatternApplicator::matchAndRewrite

       1. try to find the best pattern for the op
	 
       2. call the matchAndRewrite of the pattern object (which is a
          ConversionPattern )
	 
	      the rewriter in use is of type *ConversionPatternRewriter*,
          it listenes itself so that it can record all the changes
          made to the ir into a ConversionPatternRewriterImpl object,
          just see how many members does this type have

          an IMPORTANT step in ConversionPattern::matchAndRewrite is
          to call remapValues. it uses the type converter to calculate
          the legal type of each operand, then check if there aleady
          exist a mapping to the desired type for this operand. if
          it's not the case, it initiate a _target materializaion_
          (registered as a callback) for the operand, thus create some
          new instructions

          ConversionPatternRewriter inside has a impl class named
          *ConversionPatternRewriterImpl*, this class stores the state
          of all the modification, including:
		  
		  a) created ops
		  b) operation result replacement
		  c) block argument replacement
		  d) block actions
		  e) ignored ops
		  d) root updates
		     root updates means the op is updated in place
			 
		  these datas are updated in the notification hook of
          ConversionPatternRewriter
		  
		  a) created ops
		     ConversionPatternRewriterImpl::createdOps <- notifyOperationInserted ( create<> calls it )
		  b) operation result replacement
		     ConversionPatternRewriterImpl::replacements <- notifyOpReplaced ( replaceOp or eraseOp calls it )
		  c) block argument replacement
		     ConversionPatternRewriterImpl::argReplacements <- replaceUseOfBlockArgument ( mergeBlocks calls it )
		  d) block actions
		     ConversionPatternRewriterImpl::blockActions <- convertBlockSignature, notifyBlockIsBeingErased ( eraseBlock calls it ), 
			                                                notifyCreatedBlock ( notifyBlockCreated calls it ), notifySplitBlock ( splitBlock calls it ),
															notifyBlockBeingMerged ( mergeBlocks calls it ),
															notifyRegionIsBeingInlinedBefore ( inlineRegionBefore calls it )
		  e) ignored ops
		     ConversionPatternRewriterImpl::ignoredOps <- markNestedOpIgnored
		  d) root updates
		     ConversionPatternRewriterImpl::rootUpdates <- startRootUpdate
			 
		  number of these 6 data form *RewriteState*, and can be
          retrived via ConversionPatternRewriterImpl::getCurState().
          before we apply a pattern, we store the RewriteState as
          curState; after the pattern is applied, we get the
          RewriteState again, and refer it as nextState, the
          difference of these 2 states is the modification introduced
          by the applied pattern. if we failed to apply the pattern,
          the state is reset, otherwise, we resolve the difference of
          the two states in OperationLegalizer::legalizePatternResult
		  
		  so, is this a git like system? a stupid file system which
          only add but never modify? NO, it isn't. take root update
          for an example, the op is updated for real, and in place,
          it's just that the framework do the right bookkeeping so
          that these modification can be reverted

	   3. if the pattern is applied successfully, call
          OperationLegalizer::legalizePatternResult. what we do here
          includes:

          1. legalizePatterrnBlockActions

             legalize the block arguments in action, this basicly legalize all
             block arguments. the mapping will be added into the value mapping hold
             in the rewriter.

		  2. legalizePatterrnRootUpdates
		     call legalize() again on the updated ops

		  3. legalizePatternCreatedOperations
		     call legalize() on the new created ops
			 
		  the main purpose of this phase is:
		  a) capture uncounted value mapping
		  b) make sure everything is legalized
		  
3. after every op is converted, call OperationConverrter::finalize()
   to terminate the whole conversion
   
   this phase does ONE job, that is to make sure that the users of the
   old value being mapped get a materialization
   
   if the old value is an op result, a source materialization is
   initiated; if the old value is a block argument, an argument
   materialization is initiated

THAT'S ALL !

* Attribute Def in ODS
  - storageType C++中的mlir::Attribute类型
  - returnType C++中的Value类型
  
  这两个是Attribute定义中非常重要的两个类型。 对一个具体的op来说，在方法中可以同时得到Attribute类型的attr值，也可以得到returnType中指定的Value类型的attr值，convertFromStorage用来辅助这个工作，描述如何从storagetype得到returnType
  
  mlir-tblgen生成attribute的parsing代码时，会尝试得到literal value type。这个信息由valueType提供，这是个type constraint，如果其有非空的builderCall，那就通过builderCall得到type，否则就用Type()作为value type
  
  constBuilderCall和constant attribute相关
  
  debug: 以toy7为例，下面的命令可以打出所有的record，可以方便的看每个record的信息
  > alias tg="/home/zhujy/src/llvm-project-12.0.1.src/build/bin/mlir-tblgen -I /home/zhujy/src/llvm-project-12.0.1.src/mlir/include/ toy/Ops.td"

* Type Def in ODS
