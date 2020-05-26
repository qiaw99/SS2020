import psycopg2

def connect():
    """ Connect to the PostgreSQL database server """
    conn = None

    try:
        print('Connecting to the PostgreSQL database...')

        #connection data
        conn = psycopg2.connect(host="localhost", database="testDB", user="postgres", password="admin")

        # Open a cursor to perform database operations
        cur = conn.cursor()

        # Creating a new table
        cur.execute("CREATE TABLE test1 (id serial PRIMARY KEY, num integer, data varchar);")

        #inserting values
        cur.execute("INSERT INTO test1 (num, data) VALUES (%s, %s)", (100, "test"))

        #taking all info from the table
        cur.execute("SELECT * FROM test1;")

        #printing result
        test_table = cur.fetchone()
        print("Test table content:")
        print(test_table)

        #to make the changes persistent
        #conn.commit()

        # close the communication with the PostgreSQL
        cur.close()

    except (Exception, psycopg2.DatabaseError) as error:
        print(error)

    finally:
        if conn is not None:
            #close the communication with the Database
            conn.close()
            print('Database connection closed.')


def main():
    connect()

if __name__ == '__main__':
    main()