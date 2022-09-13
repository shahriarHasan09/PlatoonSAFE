import re



def createResultCSV(vector_file, parse_file):
    collision = False
    vectors_parsed = False
    dictionary = {}
    with open(vector_file, "r") as a_file:
        with open(parse_file, "w+") as output_file:
            output_file.writelines('"Variable", "Node", "Time", "Value"\n')
            for line in a_file:
                a = line.split()
                if len(a) > 0:
                    if a[0] == "vector":
                        vec_num = int(a[1])
                        var_name = a[3]
                        node = int(re.search(r"\d+", a[2]).group())
                        dictionary[vec_num] = [var_name,node]
                        vectors_parsed = True
                    elif vectors_parsed:
                        if vec_num != int(a[0]):
                            vec_num = int(a[0])
                            var_name, node = dictionary[vec_num]
                        
                        sim_time = float(a[2])
                        value = float(a[3])
                        if var_name == "distance" and value < 0 and node != 0:
                            collision = True
                        out_line = f"{var_name}, {node}, {sim_time}, {value}\n"
                        output_file.writelines(out_line)
    return collision
