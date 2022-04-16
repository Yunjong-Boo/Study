public class AstCall{
	public static void Call(AddNode node, int cnt){
		cnt++;
		System.out.print("ADD\n");
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.left, cnt);
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.right, cnt);
	}

	public static void Call(SubNode node, int cnt){
		cnt++;
		System.out.print("SUB\n");
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.left, cnt);
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.right, cnt);
	}

	public static void Call(MulNode node, int cnt){
		cnt++;
		System.out.print("MUL\n");
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.left, cnt);
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.right, cnt);
	}

	public static void Call(DivNode node, int cnt){
		cnt++;
		System.out.print("DIV\n");
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.left, cnt);
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.right, cnt);
	}

	public static void Call(AssignNode node, int cnt){
		cnt++;
		System.out.print("ASSIGN\n");
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.left, cnt);
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.right, cnt);
	}

	public static void Call(NegNode node, int cnt){
		System.out.print("-");
		Call(node.n, cnt);
	}

	public static void Call(NumberNode node, int cnt){
		System.out.print(node.num+"\n");
	}

	public static void Call(IDNode node, int cnt){
		System.out.print(node.ID+"\n");
	}
	
	public static void Call(minNode node, int cnt){
		cnt++;
		System.out.print("min\n");
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.left, cnt);
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.right, cnt);
	}

	public static void Call(maxNode node, int cnt){
		cnt++;
		System.out.print("max\n");
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.left, cnt);
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.right, cnt);
	}
	
	public static void Call(powNode node, int cnt){
		cnt++;
		System.out.print("pow\n");
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.left, cnt);
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.right, cnt);
	}

	public static void Call(sqrtNode node, int cnt){
		cnt++;
		System.out.print("sqrt\n");
		for(int i = 0; i < cnt; i++)	System.out.print("\t");
		Call(node.left, cnt);
	}

	public static void Call(ExprNode node, int cnt){
		if(node.getClass().getSimpleName() == "AddNode"){
			Call((AddNode)node, cnt);
		}
		else if(node.getClass().getSimpleName() == "SubNode"){
			Call((SubNode)node, cnt);
		}
		else if(node.getClass().getSimpleName() == "MulNode"){
			Call((MulNode)node, cnt);
		}
		else if(node.getClass().getSimpleName() == "DivNode"){
			Call((DivNode)node, cnt);
		}
		else if(node.getClass().getSimpleName() == "NegNode"){
			Call((NegNode)node, cnt);
		}
		else if(node.getClass().getSimpleName() == "AssignNode"){
			Call((AssignNode)node, cnt);
		}
		else if(node.getClass().getSimpleName() == "NumberNode"){
			Call((NumberNode)node, cnt);
		}
		else if(node.getClass().getSimpleName() == "IDNode"){
			Call((IDNode)node, cnt);
		}
		else if(node.getClass().getSimpleName() == "minNode"){
			Call((minNode)node, cnt);
		}
		else if(node.getClass().getSimpleName() == "maxNode"){
			Call((maxNode)node, cnt);
		}
		else if(node.getClass().getSimpleName() == "powNode"){
			Call((powNode)node, cnt);
		}
		else if(node.getClass().getSimpleName() == "sqrtNode"){
			Call((sqrtNode)node, cnt);
		}
	}
}
