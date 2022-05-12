let rec sec_last lst = 
    let len = List.length lst in
    if len < 2 then 0
    else if len = 2 then
        match lst with
        [] -> -1
        | a :: t -> a
    else
        match lst with
        [] -> -1
        | h :: lst' -> sec_last lst'
in
Format.printf "sec_last [1;2;3;4;5] => %d\n" (sec_last [1;2;3;4;5]);
Format.printf "sec_last [4;3;2;1] => %d\n" (sec_last [4;3;2;1]);
Format.printf "sec_last [] => %d\n" (sec_last []);
Format.printf "sec_last [1] => %d\n" (sec_last [1]);
Format.printf "sec_last [1;2] => %d\n" (sec_last [1;2]);
