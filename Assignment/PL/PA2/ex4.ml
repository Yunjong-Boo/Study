let merge lst1 lst2 =
    let mer = List.merge compare lst1 lst2;
    let result = List.sort mer
    print_elements result
in
merge [3;2;1][5;4]
