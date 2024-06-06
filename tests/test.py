import os
import sys
import datetime

def add_header_to_cpp_file(file_path):
    if not file_path.endswith('.cpp'):
        print("Error: This script only supports adding headers to .cpp files.")
        return

    # Get current user and last modified user
    current_user = os.system("$USER")
    last_modified_user = os.stat(file_path).st_mtime

    # Get current date and time
    current_datetime = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    # Prepare header
    header = (
        "/*\n"
        " * File: {}\n"
        " * Created by: {}\n"
        " * Last modified by: {}\n"
        " * Last modified on: {}\n"
        " */\n\n"
    ).format(os.path.basename(file_path), current_user, last_modified_user, current_datetime)

    # Read existing content of the file
    with open(file_path, 'r') as file:
        file_content = file.read()

    # Write the header and existing content back to the file
    with open(file_path, 'w') as file:
        file.write(header + file_content)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python add_header_to_cpp_file.py <file_path>")
        sys.exit(1)

    file_path = sys.argv[1]
    add_header_to_cpp_file(file_path)
    print("Header added successfully to", file_path)
