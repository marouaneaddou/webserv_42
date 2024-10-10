#!/usr/local/bin/python3
print("Content-Type: text/html")
print("\r\n\r\n")
import cgi
import cgitb
import os
import sys

cgitb.enable()

# Set up the upload directory
# You can modify this to use a specific path, or use an environment variable if available
UPLOAD_DIR = os.environ.get('UPLOAD_DIR')

def sanitize_filename(filename):
    """Sanitize the filename to prevent directory traversal and other issues."""
    return ''.join(c for c in filename if c.isalnum() or c in ['.', '-', '_']).rstrip()

def get_unique_filename(filepath):
    """Generate a unique filename if the original already exists."""
    directory, filename = os.path.split(filepath)
    name, ext = os.path.splitext(filename)
    counter = 1

    while os.path.exists(filepath):
        filepath = os.path.join(directory, f"{name}_{counter}{ext}")
        counter += 1

    return filepath

def upload_file(fileitem):
    """Handle the file upload process."""
    if not fileitem.filename:
        return "No file was uploaded."

    # Sanitize and prepare the filename
    safe_filename = sanitize_filename(os.path.basename(fileitem.filename))
    file_path = get_unique_filename(os.path.join(UPLOAD_DIR, safe_filename))

    try:
        # Ensure the upload directory exists
        os.makedirs(UPLOAD_DIR, exist_ok=True)

        # Save the file
        with open(file_path, 'wb') as f:
            f.write(fileitem.file.read())

        return f"File '{safe_filename}' uploaded successfully as '{os.path.basename(file_path)}'!"
    except IOError as e:
        return f"Failed to save the uploaded file. Error: {str(e)}"
    except Exception as e:
        return f"An unexpected error occurred: {str(e)}"

# Main execution
form = cgi.FieldStorage()

if 'file' not in form:
    print("No file was uploaded.")
else:
    result = upload_file(form['file'])
    print(result)