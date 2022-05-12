let rec gcd a b =
    if b = 0 then a
    else if a < b then gcd (b-a) a
    else gcd (a-b) b
in
Format.printf "gcd 10 0 => %d\n" (gcd 10 0);
Format.printf "gcd 9 5 => %d\n" (gcd 9 5);
Format.printf "gcd 13 13 => %d\n" (gcd 13 13);
Format.printf "gcd 37 600 => %d\n" (gcd 37 600);
Format.printf "gcd 0 0 => %d\n" (gcd 0 0);
