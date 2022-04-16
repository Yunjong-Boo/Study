class ExprNode{}

class infixExprNode extends ExprNode{
	public ExprNode left = new ExprNode();
	public ExprNode right = new ExprNode();

	public infixExprNode(){
		left = null;
		right = null;
	}
}
class AddNode extends infixExprNode{}

class SubNode extends infixExprNode{}

class MulNode extends infixExprNode{}

class DivNode extends infixExprNode{}

class NegNode extends ExprNode{
	public ExprNode n = new ExprNode();
	
	public NegNode(){
		n = null;
	}
}

class AssignNode extends ExprNode{
	public ExprNode left = new ExprNode();
	public ExprNode right = new ExprNode();

	public AssignNode(){
		left = null;
		right = null;
	}
}

class FuncNode extends ExprNode{
	public ExprNode left = new ExprNode();
	public ExprNode right = new ExprNode();
	
	public FuncNode(){
		left = null;
		right = null;
	}
}

class minNode extends FuncNode{}

class maxNode extends FuncNode{}

class powNode extends FuncNode{}

class sqrtNode extends FuncNode{}

class IDNode extends ExprNode{
	public String ID;

	public IDNode(){
		ID = null;
	}
}

class NumberNode extends ExprNode{
	public double num;

	public NumberNode(){
		num = 0;
	}
}
