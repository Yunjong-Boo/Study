import java.io.IOException;
import org.antlr.v4.runtime.*;
import java.util.Scanner;
import java.util.ArrayList;
import java.io.InputStream;
import java.io.ByteArrayInputStream;

public class program {

    public static void main(String[] args) throws IOException {
	Scanner sc = new Scanner(System.in);
	ArrayList<String> str = new ArrayList<String>();
	ArrayList<Double> result = new ArrayList<Double>();
	while(sc.hasNext()){
		String temp = sc.nextLine();
		str.add(temp);
	}

	for(int i = 0; i < str.size(); i++){
		InputStream is = new ByteArrayInputStream((str.get(i)).getBytes());

		// Get Lexer
		ExprLexer lexer = new ExprLexer(CharStreams.fromStream(is));
		// Get a list of matched tokens
		CommonTokenStream tokens = new CommonTokenStream(lexer);
		// Pass tokens to parser
		ExprParser parser = new ExprParser(tokens); 

		BuildAstVisitor ast = new BuildAstVisitor();
		ExprNode root = new ExprNode();
		root = ast.visitProg(parser.prog());

		AstCall traversal = new AstCall();
		traversal.Call(root, 0);

		Evaluate value = new Evaluate();
		result.add(value.evaluate(root));
	}

	for(double i : result){
		System.out.println(i);
	}
    }
}
