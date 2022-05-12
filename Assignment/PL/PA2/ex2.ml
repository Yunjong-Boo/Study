let prime x =
    let cnt = ref 0 in
    for i = 2 to x do
        if x mod i = 0 then cnt := !cnt + 1
        else cnt := !cnt + 0
    done;
    if !cnt = 1 then true
    else false
in
Format.printf "prime 2 => %b\n" (prime 2);
Format.printf "prime 3 => %b\n" (prime 3);
Format.printf "prime 4 => %b\n" (prime 4);
Format.printf "prime 17 => %b\n" (prime 17);
