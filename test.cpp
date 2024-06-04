#include <iostream>



int main()
{
	std::cout << "Hello\n";
}



// def add_user(conn, project):
//     sql = ''' INSERT INTO USERS(username,password,email,phone)
//               VALUES(?,?,?,?) '''
//     cur = conn.cursor()
//     cur.execute(sql, project)
//     conn.commit()
//     return cur.lastrowid

// def create_sqlite_database(filename):
//     conn = None
//     try:
//         conn = sqlite3.connect(filename)
//         curs = conn.cursor()
//         users = """ CREATE TABLE IF NOT EXISTS USERS (
//             username VARCHAR(255) NOT NULL,
//             password CHAR(25) NOT NULL,
//             email CHAR(50) NOT NULL,
//             phone CHAR(15)
//         ); """
//         curs.execute(users)
//         # project = (username, password, email, phone)
//         # user_id = add_user(conn, project)
//         # print(f'Created a user with the id {user_id}')

