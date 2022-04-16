import java.lang.Math;

class Evaluate extends AstCall {
	public static double evaluate(AddNode node){
		return (evaluate(node.left) + evaluate(node.right));
	}
	
	public static double evaluate(SubNode node){
		return (evaluate(node.left) - evaluate(node.right));
	}
	
	public static double evaluate(MulNode node){
		return (evaluate(node.left) * evaluate(node.right));
	}
	
	public static double evaluate(DivNode node){
		return (evaluate(node.left) / evaluate(node.right));
	}

	public static double evaluate(NegNode node){
		return(-evaluate(node.n));
	}

	public static double evaluate(AssignNode node){
		return (double)0;
	}

	public static double evaluate(NumberNode node){
		return node.num;
	}

	public static double evaluate(IDNode node){
		return (double)0;
	}

	public static double evaluate(minNode node){
		return Math.min(evaluate(node.left), evaluate(node.right));
	}

	public static double evaluate(maxNode node){
		return Math.max(evaluate(node.left), evaluate(node.right));
	}

	public static double evaluate(powNode node){
		return Math.pow(evaluate(node.left), evaluate(node.right));
	}

	public static double evaluate(sqrtNode node){
		return Math.sqrt(evaluate(node.left));
	}

	public static double evaluate(ExprNode node){
		String name = node.getClass().getSimpleName();

		if(name == "AddNode"){
			return evaluate((AddNode)node);
		}
		else if(name == "SubNode"){
			return evaluate((SubNode)node);
		}
		else if(name == "MulNode"){
			return evaluate((MulNode)node);
		}
		else if(name == "DivNode"){
			return evaluate((DivNode)node);
		}
		else if(name == "NegNode"){
			return evaluate((NegNode)node);
		}
		else if(name == "minNode"){
			return evaluate((minNode)node);
		}
		else if(name == "maxNode"){
			return evaluate((maxNode)node);
		}
		else if(name == "powNode"){
			return evaluate((powNode)node);
		}
		else if(name == "sqrtNode"){
			return evaluate((sqrtNode)node);
		}
		else if(name == "AssignNode"){
			return evaluate((AssignNode)node);
		}
		else if(name == "NumberNode"){
			return evaluate((NumberNode)node);
		}
		else if(name == "IDNode"){
			return evaluate((IDNode)node);
		}

		return 0;
	}
}
