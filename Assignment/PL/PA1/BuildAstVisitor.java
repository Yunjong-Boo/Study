public class BuildAstVisitor extends ExprBaseVisitor<ExprNode> {
	public ExprNode visitProg(ExprParser.ProgContext ctx){
		return visit(ctx.start);
	}

	public infixExprNode visitInfixExpr(ExprParser.InfixExprContext ctx){
		infixExprNode node = new infixExprNode();

		if(ctx.op.getType() == ExprLexer.OP_MUL){
			node = new MulNode();
		}
		else if(ctx.op.getType() == ExprLexer.OP_DIV){
			node = new DivNode();
		}
		else if(ctx.op.getType() == ExprLexer.OP_ADD){
			node = new AddNode();
		}
		else if(ctx.op.getType() == ExprLexer.OP_SUB){
			node = new SubNode();
		}

		node.left = visit(ctx.left);
		node.right = visit(ctx.right);

		return node;
	}

	public ExprNode visitUnaryExpr(ExprParser.UnaryExprContext ctx){
		NegNode node = new NegNode();
		
		if(ctx.op.getType() == ExprLexer.OP_ADD){
			return visit(ctx.expr());
		}
		else if(ctx.op.getType() == ExprLexer.OP_SUB){
			node.n = visit(ctx.expr());
			return node;
		}

		return node;
	}

	public AssignNode visitAssign(ExprParser.AssignContext ctx){
		AssignNode node = new AssignNode();

		if(ctx.op.getType() == ExprLexer.OP_ASG){
			node = new AssignNode();
		}

		node.left = visit(ctx.left);
		node.right = visit(ctx.right);

		return node;
	}

	public NumberNode visitNumber(ExprParser.NumberContext ctx){
		NumberNode node = new NumberNode();
		node.num = Double.parseDouble(ctx.n.getText());
		return node;
	}

	public IDNode visitID(ExprParser.IDContext ctx){
		IDNode node = new IDNode();
		node.ID = ctx.id.getText();
		return node;
	}

	public ExprNode visitParensExpr(ExprParser.ParensExprContext ctx){
		return visit(ctx.expr());
	}

	public FuncNode visitFuncExpr(ExprParser.FuncExprContext ctx){
		FuncNode node = new FuncNode();

		if(ctx.op.getType() == ExprLexer.OP_MIN){
			node = new minNode();
			node.left = visit(ctx.left);
			node.right = visit(ctx.right);
		}
		else if(ctx.op.getType() == ExprLexer.OP_MAX){
			node = new maxNode();
			node.left = visit(ctx.left);
			node.right = visit(ctx.right);
		}
		else if(ctx.op.getType() == ExprLexer.OP_POW){
			node = new powNode();
			node.left = visit(ctx.left);
			node.right = visit(ctx.right);
		}
		else if(ctx.op.getType() == ExprLexer.OP_SQRT){
			node = new sqrtNode();
			node.left = visit(ctx.left);
		}

		return node;
	}
}
