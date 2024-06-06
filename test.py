import sqlite3
from getpass import getpass
import sys

def add_user(conn, user):
    sql = '''INSERT INTO USERS(username, password, email, phone)
             VALUES(?, ?, ?, ?)'''
    cur = conn.cursor()
    cur.execute(sql, user)
    conn.commit()
    return cur.lastrowid

def create_sqlite_database(filename):
    conn = None
    try:
        conn = sqlite3.connect(filename)
        curs = conn.cursor()
        users = """CREATE TABLE IF NOT EXISTS USERS (
            username VARCHAR(255) NOT NULL,
            password CHAR(25) NOT NULL,
            email CHAR(50) NOT NULL,
            phone CHAR(15)
        );"""
        curs.execute(users)
    except sqlite3.Error as e:
        print(f"An error occurred: {e}")
    finally:
        if conn:
            conn.close()

def add_user(conn, project):
    sql = ''' INSERT INTO USERS(username,password,email,phone)
              VALUES(?,?,?,?) '''
    cur = conn.cursor()
    cur.execute(sql, project)
    conn.commit()
    return cur.lastrowid

def authenticate_user(conn, username, password):
    sql = '''SELECT * FROM USERS WHERE username = ? AND password = ?'''
    cur = conn.cursor()
    cur.execute(sql, (username, password))
    user = cur.fetchone()
    return user is not None

if __name__ == '__main__':
    create_sqlite_database("users.db")
    
    # print("[1] LOGIN")
    # print("[2] REGISTER")
    # response = input(">> ")
    response = sys.argv[1]
    if(response == "1"):
        print("Please enter your information")
        # username = input("[Username]> ")
        # password = getpass("[Password]🔒 ")
        username = sys.argv[2]
        password = sys.argv[3]
        print(f"{response}, {username}, {password}")
    
    else:
        print("Please enter your information")
        username = input("[Username*]> ")
        password = getpass("[Password*]🔒 ")
        confirm_password = getpass("[Confirm Password*]🔒 ")
        if(password != confirm_password):
            print(f"[!] hey {username} please enter the same passwords")
            while(password != confirm_password):
                password = getpass("[Password*]🔒 ")
                confirm_password = getpass("[Confirm Password*]🔒 ")
            
        email = input("[email*]> ")
        phone = input("[phone]> ")
        project = (username, password, email, phone)

        project = (username, password, email, phone)
        

    try:
        conn = sqlite3.connect("users.db")
        if response == "1":
            if authenticate_user(conn, username, password):
                print("[V]Logged in")
            else:
                print("[X]Error")
        elif response == "2":
            user_id = add_user(conn, project)
            print(f'Created a user with the id {user_id}')
    except sqlite3.Error as e:
        print(f"An error occurred: {e}")
    finally:
        if conn:
            conn.close()
