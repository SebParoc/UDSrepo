import subprocess

# run the parser executable
proc = subprocess.Popen(["./../cmake-build-debug-visual-studio/Debug/client.exe"], stdin=subprocess.PIPE, stdout=subprocess.PIPE)

# define the input prefix
input_prefix = ""

while True:
    # read user input
    request_arg = input("Enter request argument (or 'q' to quit): ")

    if request_arg == 'q':
        break

    # construct the full input string and write it to the subprocess stdin
    input_str = input_prefix + request_arg + "\n"
    proc.stdin.write(input_str.encode())
    proc.stdin.flush()

    # read and decode the output from the subprocess stdout
    output_bytes = proc.stdout.readline()
    output_str = output_bytes.decode().strip()

    # print the output
    print(output_str)

# close the subprocess
proc.stdin.close()
proc.stdout.close()
proc.terminate()
