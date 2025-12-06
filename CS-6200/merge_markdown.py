import os

dirs = ["P3", "P4"]
output = "gios_final_exam_notes.md"

full_contents = ""
for d in dirs:
    for sd in sorted(os.listdir(d)):
        fn = os.path.join(d, sd, "README.md")
        with open(fn, "r") as f:
            content = f.read()
        full_contents += content
        full_contents += "\n\n"

with open(output, "w") as ff:  
    ff.write(full_contents.strip())