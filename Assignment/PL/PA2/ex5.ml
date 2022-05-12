let range lower upper =
    let rec make res lower upper = 
        if lower > upper then []::res
        else make (res@lower) (lower+1) upper in
    make [] lower upper
in
range 10 15
