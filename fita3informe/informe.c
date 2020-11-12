 #include <stdio.h>
  #include <sqlite3.h>
  
  static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
  }
  
  int main(int argc, char **argv){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    int sensor = 2;
    float value_sensor = 22;
  
    if( argc!=3 ){
      fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
      return(1);
    }
    rc = sqlite3_open("basedades_adstr.db", &db);
    if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return(1);
    }
    
    char sql[1024];
    
    sprintf(sql, "Insert into mesures (id_sensor, valor) VALUES (%d, %f);", sensor, value_sensor);
    
    printf("SQL3: %s\n", sql);
    
    
    
    rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    return 0;
  }


																		/*arm-linux-gnueabihf-gcc informe.c -o sql -L. -lsqlite3
																		./sql basedades_adstr.db "select * from mesures"
																		*/
