module F = Format

let rec interp_expr (e: Ast.expr) (g: FStore.t) (s: Store.t) : Value.t = 
  (* Implement this function *)
  match e with
  | Num n -> NumV n
  | Add (e1, e2) -> 
    begin
      match e1 with
      | Num n1 -> 
        begin
          match e2 with
          | Num n2 -> NumV(n1 + n2)
          | _ -> interp_expr e2 g s
        end
      | _ -> interp_expr e1 g s
    end
  | Sub (e1, e2) ->
    begin
      match e1 with
      | Num n1 -> 
        begin
          match e2 with
          | Num n2 -> NumV(n1 - n2)
          | _ -> interp_expr e2 g s
        end
      | _ -> interp_expr e1 g s
    end
  | Id x -> Store.find x s
  | LetIn (x, e1, e2) -> interp_expr e2 g (Store.add x (interp_expr e1 g s) s)
  | Call (x, el) -> 
    begin
      match (FStore.find x g) with
      | (param, body) ->
        begin
          let rec storeData param el s =
            match param, el with
            | para::param_t, e::el_t -> storeData param_t el_t (Store.add para (interp_expr e g s) s)
            | ([] , _::_) -> failwith "Free identifier"
            | (_::_ , []) -> failwith "The number of arguments of x mismatched: Required: x, Actual: x"
            | ([], []) -> s
          in
          interp_expr body g (storeData param el s)
        end
    end


let interp_fundef (d: Ast.fundef) (g: FStore.t) : FStore.t = 
  (* Implement this function *)
  match d with
  | FunDef (str, param_list, e) -> FStore.add str (param_list, e) g


let interp (p: Ast.prog) : Value.t = 
  (* Implement this function *)
  let g = FStore.empty in
    match p with
    | Prog (dl, e) -> 
      begin
        let rec interprete dl g = 
          match dl with
          | [] -> g
          | h::t -> interprete t (interp_fundef h g)
        in
        interp_expr e (interprete dl g) Store.empty
      end
