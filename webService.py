#!/usr/bin/python2.7
#coding:utf-8
#
from flask import Flask,request,g,make_response,jsonify
from werkzeug.security import generate_password_hash
from werkzeug.security import check_password_hash
import MySQLdb
import json
import os
import time
import io
import ctypes,math,urllib
so=ctypes.cdll.LoadLibrary
lib=so("/usr/lib/libSemiHomo.so")
pswKey={}
host_='202.120.39.21'
user_='root'
passwd_='letmein'
db_='DemoAuxDB'
con = MySQLdb.connect(host=host_,user=user_,passwd=passwd_,db=db_)
app = Flask(__name__)

#hello
@app.route('/todo/api/v1.0/hello',methods=['POST','GET'])
def hello():
    if request.method == 'GET':
        text = request.args.get('text')
    if request.method == 'POST':
        text = request.form['text']
    return 'hello, %s'%text
       
#connection
@app.route('/todo/api/v1.0/connection', methods=['POST','GET'])
def connection():
   if request.method == 'GET':
      host = request.args.get('host')
      user = request.args.get('user')
      passwd = request.args.get('passwd')
      port = request.args.get('port')
      port = port.encode('ascii')
      #dbname=request.args.get('dbname')      
      db= MySQLdb.connect(host=host,user=user,passwd=passwd,port=int(port))
   if request.method == 'POST':
      host = request.form['host']
      user = request.form['user']
      passwd = request.form['passwd']
      port = request.form['port']
      #dbname=request.args.get('dbname')
      port = port.encode('ascii')
#      print port
#      print type(port)
      db= MySQLdb.connect(host=host,user=user,passwd=passwd,port=int(port))
     
   return db

def get_db():
    if not hasattr(g,'db'):
       g.db=connection()
       print hasattr(g,'db') 
       rm_file("/home/rootagent/cryptdb/mysqlproxy/test.txt")
    return g.db

#createdb
@app.route('/todo/api/v1.0/CreateDB', methods=['POST','GET'])
def CreateDB():
   
    db = get_db()
    cursor = db.cursor()
    if request.method == 'GET':
       dbname=request.args.get('dbname')
       sql2="create database"+' '+dbname      
       try:
          cursor.execute(sql2)
          db.commit()
       finally:
          rm_file("/home/rootagent/cryptdb/mysqlproxy/test.txt")             
          return "ok"
    if request.method == 'POST':
       dbname=request.form['dbname']
       sql2="create database"+' '+dbname
       try:
          cursor.execute(sql2)
          db.commit()
       finally:
          rm_file("/home/rootagent/cryptdb/mysqlproxy/test.txt")
          return "ok"
#Register
@app.route('/todo/api/v1.0/Register',methods=['POST','GET'])
def Register():
    if request.method == 'GET':
        user=request.args.get('USER')
        PWD=request.args.get('PWD')
        password_hash=generate_password_hash(PWD)
        print password_hash
        cur=con.cursor()
       # con = MySQLdb.connect(host=host_,user=user_,passwd=passwd_,db=db_)
        sql1="use DemoAuxDB"
        cur.execute(sql1)
        con.commit()
        sql = "insert into UserInfo values("+"\""+user+"\","+"\""+password_hash+"\""+')'
        print sql
        try:
            cur.execute(sql)
            con.commit()
            resultDict = {'status':"success"}
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
            resultDict = {'status':"failed"}
        finally:
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            print r
            return r

    if request.method == 'POST':
        user=request.form['USER']
        PWD=request.form['PWD']
        password_hash=generate_password_hash(PWD)
        print password_hash
        cur=con.cursor()
       # con = MySQLdb.connect(host=host_,user=user_,passwd=passwd_,db=db_)
        sql1="use DemoAuxDB"
        cur.execute(sql1)
        con.commit()
        sql = "insert into UserInfo values("+"\""+user+"\","+"\""+password_hash+"\""+')'       
        try:
            cur.execute(sql)
            con.commit()
            resultDict = {'status':"success"}
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
            resultDict = {'status':"failed"}
        finally:
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            print r
            return r
#Login
@app.route('/todo/api/v1.0/Login',methods=['POST','GET'])
def Login():
    if request.method == 'GET':
        user=request.args.get('USER')
        user=str(user)
        PWD=request.args.get('PWD')
        password_hash=generate_password_hash(PWD)
        print password_hash
        cur=con.cursor()
       # con = MySQLdb.connect(host=host_,user=user_,passwd=passwd_,db=db_)
        sql1="use DemoAuxDB"
        cur.execute(sql1)
        con.commit()
        sql = "select PWDINFO from UserInfo where USER="+'\''+user+'\''
        print sql
        try:
            cur.execute(sql)
            con.commit()
            results = cur.fetchall()
            print "results:"
            resultlist=list(results)
            a=str(resultlist[0])
            r=a[2:]
            rr=r[:-3]
            print rr
            if check_password_hash(rr,PWD): 
               resultDict = {'status':"success"}
            else:
               resultDict = {'status':"failed"}
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
            resultDict = {'status':"failed"}
        finally:
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            print r
            return r

    if request.method == 'POST':
        user=request.form['USER']
        PWD=request.form['PWD']
        password_hash=generate_password_hash(PWD)
        print password_hash
        cur=con.cursor()
       # con = MySQLdb.connect(host=host_,user=user_,passwd=passwd_,db=db_)
        sql1="use DemoAuxDB"
        cur.execute(sql1)
        con.commit()
        sql = "select PWDINFO from UserInfo where USER="+'\''+user+'\''
        print sql
        try:
            cur.execute(sql)
            con.commit()
            results = cur.fetchall()
            print "results:"
            resultlist=list(results)
            a=str(resultlist[0])
            r=a[2:]
            rr=r[:-3]
            print rr
            if check_password_hash(rr,PWD):
               resultDict = {'status':"success"}
            else:
               resultDict = {'status':"failed"}
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
            resultDict = {'status':"failed"}
        finally:
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            print r
            return r
#disconnection
@app.teardown_appcontext
def Disconnection(error):
    """Closes the database again at the end of the request."""
    if hasattr(g, 'db'):
        g.db.close()
    return "ok"
	  
#createtable
@app.route('/todo/api/v1.0/CreateTable',methods=['POST','GET'])
def CreateTable():
    db = get_db()
    cursor = db.cursor()
    if request.method == 'GET':
       dbname=request.args.get('dbname')
       sql_1="use"+' '+dbname
       cursor.execute(sql_1)
       tablename=request.args.get('tablename')
       tableColumns=request.args.get('tableColumns')
       
       sql_2="create table"+' '+tablename+"("+tableColumns+")"
       cursor.execute(sql_2)
       db.commit()
       return "ok"
    if request.method == 'POST':
       dbname=request.form['dbname']
       sql_1="use"+' '+dbname
       cursor.execute(sql_1)
       tablename=request.form['tablename']
       tableColumns=request.form['tableColumns']
       sql_2="CREATE TABLE"+' '+tablename+"("+tableColumns+")"
       time_in=time.time()
       cursor.execute(sql_2)
       db.commit()
       time_out=time.time()
       execution_time=time_out-time_in
       print 'execution_time='+str(execution_time)
       resultDict = {'result':"ok",'execution_time':execution_time*1000}
       r = make_response(jsonify(resultDict))
       r.headers["Access-Control-Allow-Origin"] =  "*"
       print r
       return r

#Insert       
@app.route('/todo/api/v1.0/Insert',methods=['POST','GET'])
def Insert():
    db =get_db()
    cursor = db.cursor()
    if request.method == 'GET':
       dbname=request.args.get('dbname')
       cursor.execute("use"+' '+dbname)
       tablename=request.args.get('tablename')
       field=request.args.get('field')
       values = request.args.get('values')
       sql="insert into"+' '+tablename+'('+field+')'+' '+"values"+'('+values+')'
       cursor.execute(sql)
       db.commit()
       return 'ok'
    if request.method =='POST':
       dbname=request.form['dbname']
       dbname=str(dbname)
       cursor.execute("use"+' '+dbname)
       tablename=request.form['tablename']
       tablename=str(tablename)
       field=request.form['field']
       field=str(field)
       values = request.form['values']
       values=str(values)
       sql="insert into"+' '+tablename+'('+field+')'+' '+"values"+'('+values+')'+';'
#       sql="INSERT INTO"+' '+tablename +' '+"VALUES"+'('+values+')'+";"
       time_in=time.time()
       cursor.execute(sql)
       db.commit()
       time_out=time.time()
       execution_time=time_out-time_in
       print execution_time
       resultDict = {'result':"ok",'execution_time':execution_time*1000}
       r = make_response(jsonify(resultDict))
       r.headers["Access-Control-Allow-Origin"] =  "*"
       return r
#update
@app.route('/todo/api/v1.0/Update',methods=['POST','GET'])
def Update():
    db=get_db()
    cursor = db.cursor()
    if request.method == 'GET':
       dbname=request.args.get('dbname')
       cursor.execute("use"+' '+dbname)
       tablename=request.args.get('tablename')
       field =request.args.get('field')
       condition=request.args.get('condition')
       print condition
       print field
       if(condition):

           sql='UPDATE'+' '+tablename+' '+'SET'+' '+field+' '+'WHERE '+condition+';'
       else:
           sql='UPDATE'+' '+tablename+' '+'SET'+' '+field
       print sql
       cursor.execute(sql)
       db.commit()
       return "ok"
    if request.method == 'POST':
       dbname=request.form['dbname']
       cursor.execute("use"+' '+dbname)
       tablename=request.form['tablename']
       condition=request.form['condition']
       field =request.form['field']
       values=request.form['values']
       if(condition):

           sql='UPDATE'+' '+tablename+' '+'SET'+' '+field+'='+values+' '+'WHERE '+condition+';'
       else:
           sql='UPDATE'+' '+tablename+' '+'SET'+' '+field+'='+values


       time_in=time.time()
       cursor.execute(sql)
       db.commit()
       time_out=time.time()
       execution_time=time_out-time_in
       print execution_time
       resultDict = {'result':"ok",'execution_time':execution_time*1000}
       r = make_response(jsonify(resultDict))
       r.headers["Access-Control-Allow-Origin"] =  "*"
       return r
#delete
@app.route('/todo/api/v1.0/Delete',methods=['POST','GET'])
def Delete():
    db=get_db()
    cursor = db.cursor()
    if request.method == 'GET':
       dbname=request.args.get('dbname')
       cursor.execute("use"+' '+dbname)
       tablename=request.args.get('tablename')
       field=request.args.get('field')
       sql="delete"+' '+"from"+' '+tablename+' '+"where"+' '+field+';'
       cursor.execute(sql)
       db.commit()
       return "ok"
    if request.method == 'POST':
       dbname=request.form['dbname']
       cursor.execute("use"+' '+dbname)
       tablename=request.form['tablename']
       condition=request.form['condition']
       if(condition):

           sql="DELETE"+' '+"FROM"+' '+tablename+' '+"WHERE"+' '+condition+';'
       else:
           sql="drop"+' '+'table'+' '+tablename
       print sql
       time_in=time.time()
       cursor.execute(sql)
       db.commit()
       time_out=time.time()
       execution_time=time_out-time_in
       print execution_time
       resultDict = {'result':"ok",'execution_time':execution_time*1000}
       r = make_response(jsonify(resultDict))
       r.headers["Access-Control-Allow-Origin"] =  "*"
       return r

#qingkongwenjianneirong
def rm_file(filename):
    f = open(filename, "w+")
    f.truncate()
    f.flush()
    f.close()
    return "ok!"
#huodewenjianneirong
def get_text_file(filename):
    if not os.path.exists(filename):
        print("ERROR: file not exit: %s" % (filename))
	return None

    if not os.path.isfile(filename):
	print("ERROR: %s not a filename." % (filename))
	return None

    f = open(filename, "r+")
    content = f.read()
    f.close()
    return content
#querywithencryption
@app.route('/todo/api/v1.0/QueryWithEncryption',methods=['POST','GET'])
def QueryWithEncryption():
    db=get_db()
    cursor = db.cursor()
    rm_file("/home/rootagent/cryptdb/mysqlproxy/test.txt")
    if request.method == 'GET':
       dbname=request.args.get('dbname')
       cursor.execute("use"+' '+dbname)
       tablename=request.args.get('tablename')
       field=request.args.get('field')
       condition=request.args.get('condition')
       if(condition):
           sql="select"+' '+field+' '+"from"+' '+tablename+' '+'where'+' '+condition
       else:
           sql="select"+' '+field+' '+"from"+' '+tablename
#      if  os.path.getsize("/home/alice/cryptdb/mysqlproxy/test.txt"):
#      rm_file("/home/alice/cryptdb/mysqlproxy/test.txt")
       time_in=time.time()
       try:
           cursor.execute(sql)
           db.commit()
       except:
#          rm_file("/home/alice/cryptdb/mysqlproxy/test.txt")
           db.rollback()
       finally:
           content= get_text_file("/home/rootagent/cryptdb/mysqlproxy/test.txt") 
           time_out=time.time()
           execution_time=time_out-time_in
           print execution_time
           print content
           rm_file("/home/rootagent/cryptdb/mysqlproxy/test.txt")
           return (content,execution_time)


    if request.method == 'POST':
       dbname=request.form['dbname']
       cursor.execute("use"+' '+dbname)
       tablename=request.form['tablename']
       field=request.form['field']
       condition=request.form['condition']
       if(condition):
           sql="select"+' '+field+' '+"from"+' '+tablename+' '+'where'+' '+condition
       else:
           sql="select"+' '+field+' '+"from"+' '+tablename


#      if  os.path.getsize("/home/alice/cryptdb/mysqlproxy/test.txt"):
#      rm_file("/home/alice/cryptdb/mysqlproxy/test.txt")
       
       print sql
       try:
           time_in=time.time()
           cursor.execute(sql)
           db.commit()
           time_out=time.time()
           execution_time=time_out-time_in
           print execution_time
       except:
#          rm_file("/home/alice/cryptdb/mysqlproxy/test.txt")
           db.rollback()
       finally:
           content= get_text_file("/home/rootagent/cryptdb/mysqlproxy/test.txt")
           print content
           rm_file("/home/rootagent/cryptdb/mysqlproxy/test.txt")
           
           resultList = [{'content':content},]
           dictionary ={'result':resultList,'execution_time':execution_time*1000}
           r = make_response(jsonify(dictionary))
           r.headers["Access-Control-Allow-Origin"] =  "*"
           return r
#querywithplaintext
@app.route('/todo/api/v1.0/QueryWithPlaintext',methods=['POST','GET'])       
def QueryWithPlaintext():
    db=get_db()
    cursor = db.cursor()
    if request.method == 'GET':
       dbname=request.args.get('dbname')
       cursor.execute("use"+' '+dbname)
       tablename=request.args.get('tablename')
       field=request.args.get('field')
       condition=request.args.get('condition')
       if(condition):
           sql="select"+' '+field+' '+"from"+' '+tablename+' '+'where'+' '+condition
       else:
           sql="select"+' '+field+' '+"from"+' '+tablename
       cursor.execute(sql)
       results = cursor.fetchall()
       return  str(results)
    if request.method == 'POST':
       dbname=request.form['dbname']
       cursor.execute("use"+' '+dbname)
       tablename=request.form['tablename']
       field=request.form['field']
       condition=request.form['condition']
       if(condition):
           sql="select"+' '+field+' '+"from"+' '+tablename+' '+'where'+' '+condition
       else:
           sql="select"+' '+field+' '+"from"+' '+tablename
       time_in=time.time()
       cursor.execute(sql)
       results = cursor.fetchall()
       time_out=time.time()
       execution_time=time_out-time_in
       print execution_time
       resultList = list(results)
       resultDict = {'result':resultList,'execution_time':execution_time*1000}
       r = make_response(jsonify(resultDict))
       r.headers["Access-Control-Allow-Origin"] =  "*"
       return r


#KeyGeneration
@app.route('/todo/api/v1.0/KeyGeneration',methods=['POST','GET'])
def KeyGeneration():
    if request.method == 'POST':
            pKey=str(request.form['key'])
            pswKey['foo']=pKey
            print pswKey['foo']
            type_=request.form['type']
	    time_in=time.time()
            a=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
	    time_out=time.time()
	    execution_time=time_out-time_in
            if(a):
	        resultDict = {'value':"ok",'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r  
	    else:
                resultDict = {'value':"false",'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r  
#InsertExecuteTime
@app.route('/todo/api/v1.0/InsertExecuteTime',methods=['POST','GET'])
def InsertExecuteTime():
    if request.method == 'POST':
        type_=request.form['type']
        TimeSpanInfo=request.form['TimeSpanInfo']
        type_=str(type_)
        TimeSpanInfo=str(TimeSpanInfo)
        cur=con.cursor()
        sql1="use DemoAuxDB"
        cur.execute(sql1)
        con.commit()
        sql = "insert into  ExpRecordLog(OpType,TimeSpanInfo) values("+type_+','+TimeSpanInfo+')'
        print sql
        try:
            cur.execute(sql)
            con.commit()
            resultDict = {'status':"success"}
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
            resultDict = {'status':"failed"}
        finally:
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            print r
            return r	           
    if request.method == 'GET':
        type_=request.args.get('type')
        TimeSpanInfo=request.args.get('TimeSpanInfo')
        type_=str(type_)
      #  TimeSpanInfo=str(TimeSpanInfo)
        cur=con.cursor()
        sql1="use DemoAuxDB"
        cur.execute(sql1)
        con.commit()
        sql = "insert into  ExpRecordLog(OpType,TimeSpanInfo) values("+type_+','+TimeSpanInfo+')'
        print sql
        try:
            cur.execute(sql)
            con.commit()
            resultDict = {'status':"success"}
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
            resultDict = {'status':"failed"}
        finally:
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            print r
            return r

#QueryExecuteTime
@app.route('/todo/api/v1.0/QueryExecuteTime',methods=['POST','GET'])
def QueryExecuteTime():
    if request.method == 'GET':
        DateTime1=request.args.get('DateTime1')
        DateTime1=str(DateTime1)
        DateTime2=request.args.get('DateTime2')
        DateTime2=str(DateTime2)
        Optype=request.args.get('Optype')
        Optype=str(Optype)
        cur=con.cursor()
        if DateTime1=='ALL' and Optype=='ALL':
            sql="select Optype, TimeSpanInfo from ExpRecordLog"
        elif DateTime1=='ALL' and Optype!='ALL':
            sql="select Optype, TimeSpanInfo from ExpRecordLog where Optype="+'\''+Optype+'\''
        elif DateTime1!='ALL' and  Optype=='ALL':
            sql="select Optype, TimeSpanInfo from ExpRecordLog where DateTime between"+' '+'\''+DateTime1+'\''+' '+"and"+' '+'\''+DateTime2+'\''+' '
        else:
            sql="select Optype, TimeSpanInfo from ExpRecordLog where DateTime between"+' '+'\''+DateTime1+'\''+' '+"and"+' '+'\''+DateTime2+'\''+' '+"and"+' '"Optype="+' '+'\''+Optype+'\''
        sql1="use DemoAuxDB"
        cur.execute(sql1)
        con.commit()
        print sql
        try:
            cur.execute(sql)
            con.commit()
            results = cur.fetchall()
            print "results:"
            resultlist=list(results)
            
            resultDict = {'TimeSpanInfoList':resultlist}
        except:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
            resultDict = {'TimeSpanInfoList':'NULL'}
        finally:
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            print r
            return r

    if request.method == 'POST':
        DateTime1=request.form['DateTime1'] 
        DateTime1=str(DateTime1)
        DateTime2=request.form['DateTime2']
        DateTime2=str(DateTime2)
        Optype=request.form['Optype']
        Optype=str(Optype)
        cur=con.cursor()
        if DateTime1=='ALL' and Optype=='ALL':
            sql="select Optype, TimeSpanInfo from ExpRecordLog"
        elif DateTime1=='ALL' and Optype!='ALL':
            sql="select Optype, TimeSpanInfo from ExpRecordLog where Optype="+'\''+Optype+'\''
        elif DateTime1!='ALL' and  Optype=='ALL':
            sql="select Optype, TimeSpanInfo from ExpRecordLog where DateTime between"+' '+'\''+DateTime1+'\''+' '+"and"+' '+'\''+DateTime2+'\''+' '
        else:
            sql="select Optype, TimeSpanInfo from ExpRecordLog where DateTime between"+' '+'\''+DateTime1+'\''+' '+"and"+' '+'\''+DateTime2+'\''+' '+"and"+' '"Optype="+' '+'\''+Optype+'\''
        sql1="use DemoAuxDB"
        cur.execute(sql1)
        con.commit()
        print sql
        try:
            cur.execute(sql)
            con.commit()
            results = cur.fetchall()
            print "results:"
            resultlist=list(results)

            resultDict = {'TimeSpanInfoList':resultlist}
        except:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
            resultDict = {'TimeSpanInfoList':'NULL'}
        finally:
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            print r
            return r

#QueryAveData
@app.route('/todo/api/v1.0/QueryAveData',methods=['POST','GET'])
def QueryAveData():
    if request.method == 'POST':
        field=request.form['field']
        condition=request.form['condition']
        cur=con.cursor()
        sql1="use DemoAuxDB"
        cur.execute(sql1)
        con.commit()
        if (condition):
            sql="select"+' '+field+' '+"from EncData"+' '+"where"+' '+condition
        else:
            sql="select"+' '+field+' '+"from EncData"
        print sql
        try:
            cur.execute(sql)
            con.commit()
            results = cur.fetchall()
            resultlist=list(results)
            resultstr1=str(resultlist)
            resultstr2=resultstr1.replace("\',","\'")
            resultstr3=resultstr2.replace('(\'','')
            resultstr=resultstr3.replace('\')','')
            print resultstr
            f1=resultstr[1:]
            f2=f1[:-1]
            gg=f2.split(',')
            sout=ctypes.create_string_buffer(10000)
            c=lib.AveSemiHomoCipherCalculate(sout,resultstr)
            if (c):
                print c
                print "avecalculatecipherresult:"
                ps=repr(sout.value)
                pf=ps[1:]
                pp=pf[:-1]
                print pp
                resultDict = {'result':pp,'data':gg}
            else:
                resultDict = {'result':'false','data':gg}
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
            resultDict = {'result':'false','data':'NULL'}
        finally:
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            print r
            return r

    if request.method == 'GET':
        field=request.args.get('field')
        condition=request.args.get('condition')
        cur=con.cursor()
        sql1="use DemoAuxDB"
        cur.execute(sql1)
        con.commit()
        if (condition):
            sql="select"+' '+field+' '+"from EncData"+' '+"where"+' '+condition
        else:
            sql="select"+' '+field+' '+"from EncData"
        print sql
        try:
            cur.execute(sql)
            con.commit()
            results = cur.fetchall()
            resultlist=list(results)
            resultstr1=str(resultlist)
            resultstr2=resultstr1.replace("\',","\'")
            resultstr3=resultstr2.replace('(\'','')
            resultstr=resultstr3.replace('\')','')
            print resultstr
            f1=resultstr[1:]
            f2=f1[:-1]
            gg=f2.split(',')
            sout=ctypes.create_string_buffer(10000)
            c=lib.AveSemiHomoCipherCalculate(sout,resultstr)
            if (c):
                print c
                print "avecalculatecipherresult:"
                ps=repr(sout.value)
                pf=ps[1:]
                pp=pf[:-1]
                print pp
                resultDict = {'result':pp,'data':gg}
            else:
                resultDict = {'result':'false','data':gg}
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
            resultDict = {'result':'false','data':'NULL'}
        finally:
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            print r
            return r
         
#QueryStdData
@app.route('/todo/api/v1.0/QueryStdData',methods=['POST','GET'])
def QueryStdData():
    if request.method == 'POST':
        field=request.form['field']
        condition=request.form['condition']
        cur=con.cursor()
        sql1="use DemoAuxDB"
        cur.execute(sql1)
        con.commit()
        if (condition):
            sql="select"+' '+field+' '+"from EncData"+' '+"where"+' '+condition
        else:
            sql="select"+' '+field+' '+"from EncData"
        print sql
        try:
            cur.execute(sql)
            con.commit()
            results = cur.fetchall()
            resultlist=list(results)
            resultstr1=str(resultlist)
            resultstr2=resultstr1.replace("\',","\'")
            resultstr3=resultstr2.replace('(\'','')
            resultstr=resultstr3.replace('\')','')
            print resultstr
            f1=resultstr[1:]
            f2=f1[:-1]
            gg=f2.split(',')

            sout=ctypes.create_string_buffer(10000)
            c=lib.VarSemiHomoCipherCalculate(sout,resultstr)
            if (c):
                print c
                print "varcalculatecipherresult:"
                ps=repr(sout.value)
                pf=ps[1:]
                pp=pf[:-1]
                print pp
                resultDict = {'result':pp,'data':gg}
            else:
                resultDict = {'result':'false','data':gg}
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
            resultDict = {'result':'false','data':'NULL'}
        finally:
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            print r
            return r
    if request.method == 'GET':
        field=request.args.get('field')
        condition=request.args.get('condition')
        cur=con.cursor()
        sql1="use DemoAuxDB"
        cur.execute(sql1)
        con.commit()
        if (condition):
            sql="select"+' '+field+' '+"from EncData"+' '+"where"+' '+condition
        else:
            sql="select"+' '+field+' '+"from EncData"
        print sql
        try:
            cur.execute(sql)
            con.commit()
            results = cur.fetchall()
            resultlist=list(results)
            resultstr1=str(resultlist)
            resultstr2=resultstr1.replace("\',","\'")
            resultstr3=resultstr2.replace('(\'','')
            resultstr=resultstr3.replace('\')','')
            print resultstr
            f1=resultstr[1:]
            f2=f1[:-1]
            gg=f2.split(',')

            sout=ctypes.create_string_buffer(10000)
            c=lib.VarSemiHomoCipherCalculate(sout,resultstr)
            if (c):
                print c
                print "varcalculatecipherresult:"
                ps=repr(sout.value)
                pf=ps[1:]
                pp=pf[:-1]
                print pp
                resultDict = {'result':pp,'data':gg}
            else:
                resultDict = {'result':'false','data':gg}
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
            resultDict = {'result':'false','data':'NULL'}
        finally:
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            print r
            return r
            
#QuerySortData
@app.route('/todo/api/v1.0/QuerySortData',methods=['POST','GET'])
def QuerySortData():
    if request.method == 'POST':
        field=request.form['field']
        condition=request.form['condition']
        sql1="use DemoAuxDB"
        cur=con.cursor()
        cur.execute(sql1)
        con.commit()
        if (condition):
            sql="select"+' '+field+' '+"from EncData"+' '+"where"+' '+condition
        else:
            sql="select"+' '+field+' '+"from EncData"
        print sql
        try:
            cur.execute(sql)
            con.commit()
            results = cur.fetchall()
            resultlist=list(results)
            resultstr=str(resultlist)
            resultstr2=resultstr1.replace("\',","\'")
            resultstr3=resultstr2.replace('(\'','')
            resultstr=resultstr3.replace('\')','')

            print resultstr
            f1=resultstr[1:]
            f2=f1[:-1]
            gg=f2.split(',')

            sout=ctypes.create_string_buffer(10000)
            c=lib.HomSort(sout,resultstr)
            if (c):
                print c
                print "sortresult:"
                ps=repr(sout.value)
                pf=ps[2:]
                pp=pf[:-2]
                pg=pp.split(',')
                print pg
                resultDict = {'result':pg,'data':gg}
            else:
                resultDict = {'result':'false','data':gg}
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
            resultDict = {'result':'false','data':'NULL'}
        finally:
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            print r
            return r
    if request.method == 'GET':
        field=request.args.get('field')
        condition=request.args.get('condition')
        sql1="use DemoAuxDB"
        cur=con.cursor()
        cur.execute(sql1)
        con.commit()
        if (condition):
            sql="select"+' '+field+' '+"from EncData"+' '+"where"+' '+condition
        else:
            sql="select"+' '+field+' '+"from EncData"
        print sql
        try:
            cur.execute(sql)
            con.commit()
            results = cur.fetchall()
            resultlist=list(results)
            resultstr1=str(resultlist)
            resultstr2=resultstr1.replace("\',","\'")
            resultstr3=resultstr2.replace('(\'','')
            resultstr=resultstr3.replace('\')','')
            print resultstr
            f1=resultstr[1:]
            f2=f1[:-1]
            gg=f2.split(',')
            sout=ctypes.create_string_buffer(10000)
            c=lib.HomSort(sout,resultstr)
            if (c):
                print c
                print "sortresult:"
                ps=repr(sout.value)
                pf=ps[2:]
                pp=pf[:-2]
                pg=pp.split(',')
                print pg
                resultDict = {'result':pg,'data':gg}
            else:
                resultDict = {'result':'false','data':gg}
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
            resultDict = {'result':'false','data':'NULL'}
        finally:
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            print r
            return r

#QueryLinearRegressionData
@app.route('/todo/api/v1.0/QueryLinearRegressionData',methods=['POST','GET'])
def QueryLinearRegressionData():
    if request.method == 'POST':
        #field1=request.form['field1']
        #field2=request.form['field2']
        condition=request.form['condition']
        #field1=str(field1)
        #field2=str(field2)
        cur=con.cursor()
        condition=str(condition)
        sql1="use DemoAuxDB"
        cur.execute(sql1)
        con.commit()
        if (condition!='None'):
            sql="select Xdata,Ydata from EncData where "+condition
        else:
            sql="select Xdata,Ydata from EncData"
        print sql
        cur.execute(sql)
        con.commit()
        results = cur.fetchall()
        resultlist=list(results)
        resultstr1=str(resultlist)
        resultstr=resultstr1.replace('\'','')
        print resultstr
        p1=resultstr[1:]
        p2=p1[:-1]
        p3=p2.replace('(','{')
        p4=p3.replace(')','}')
        p5=p4.replace(', ',' ')
        p6=p5.replace('} ','},')
        p7=p6.replace('{','{\'x\':')
        p8=p7.replace(' ',',\'y\':')
        p9=p8.replace('},','}, ')
        p10=p9.split(', ')
        for i in range(len(p10)):
            p10[i]=p10[i].replace('\'x\':','\'x\':\'')
            p10[i]=p10[i].replace(',\'y\':','\',\'y\':\'')
            p10[i]=p10[i].replace('}','\'}')
            p10[i]=eval(p10[i])
            print type(p10[i])
        sout1=ctypes.create_string_buffer(10000)
        sout2=ctypes.create_string_buffer(10000)
        c=lib.LinearRegressionSemiHomoCipherCalculate(sout1,sout2,resultstr)
        if (c):
            print repr(sout1.value)
            print repr(sout2.value)
            resultDict = {'slope':repr(sout1.value),'intercept':repr(sout2.value),'data':p10}
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            return r
        else:
            resultDict = {'slope':'false','intercept':'false','execution_time':execution_time*1000}
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            return r
    if  request.method == 'GET':
       # field1=request.form['field1']
       # field2=request.form['field2']
        condition=request.args.get('condition')
       # field1=str(field1)
       # field2=str(field2)
        cur=con.cursor()
        condition=str(condition)
        sql1="use DemoAuxDB"
        cur.execute(sql1)
        con.commit()
        if (condition!='None'):
            sql="select Xdata,Ydata from EncData where "+condition
        else:
            sql="select Xdata,Ydata from EncData"
        print sql
       # try:
        cur.execute(sql)
        con.commit()
        results = cur.fetchall()
        resultlist=list(results)
        resultstr1=str(resultlist)
        resultstr=resultstr1.replace('\'','')
        print resultstr
        p1=resultstr[1:]
        p2=p1[:-1]
        p3=p2.replace('(','{')
        p4=p3.replace(')','}')
        p5=p4.replace(', ',' ')
        p6=p5.replace('} ','},')
        p7=p6.replace('{','{\'x\':')
        p8=p7.replace(' ',',\'y\':')
        p9=p8.replace('},','}, ') 
        p10=p9.split(', ')
        for i in range(len(p10)):
            p10[i]=p10[i].replace('\'x\':','\'x\':\'')
            p10[i]=p10[i].replace(',\'y\':','\',\'y\':\'')  
            p10[i]=p10[i].replace('}','\'}')
            p10[i]=eval(p10[i])
            print type(p10[i])
        sout1=ctypes.create_string_buffer(10000)
        sout2=ctypes.create_string_buffer(10000)
        c=lib.LinearRegressionSemiHomoCipherCalculate(sout1,sout2,resultstr) 
        if (c):
            print repr(sout1.value)
            print repr(sout2.value)
            resultDict = {'slope':repr(sout1.value),'intercept':repr(sout2.value),'data':p10}
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            return r
        else:
            resultDict = {'slope':'false','intercept':'false','execution_time':execution_time*1000}
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            return r            
#HomEncryption
@app.route('/todo/api/v1.0/HomEncryption',methods=['POST','GET'])
def HomEncryption():
    if request.method == 'POST':
        value=request.form['value']
        value=str(value)
        print value
        time_in=time.time()
	sout=ctypes.create_string_buffer(10000)
        print pswKey['foo']
	#lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        result=lib.PlainEncrypt(sout,value)
        time_out=time.time()
        execution_time=time_out-time_in
	if (result):
            s=repr(sout.value)
            f=s[:-1]
            resultDict = {'value':f[1:],'execution_time':execution_time*1000}
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            return r
	else:
            resultDict = {'value':'false','execution_time':execution_time*1000}
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            return r
    if request.method == 'GET':
        a=request.args.get('value')
        print a
        time_in=time.time()
        sout=ctypes.create_string_buffer(20000)
        lib.SetAgentKeyFromPsw_(pswKey,14)
        result=lib.PlainEncrypt(sout,a)
        time_out=time.time()
        execution_time=time_out-time_in
        if (result):
            return repr(sout.value)
        else:
            
            return "false"
   

#HomDecryption
@app.route('/todo/api/v1.0/HomDecryption',methods=['POST','GET'])
def HomDecryption():
    if request.method == 'POST':
        a=request.form['value']
        b=str(a)
        time_in=time.time()
	sout=ctypes.create_string_buffer(10000)
	#lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        result=lib.CipherDecrypt(sout,b)
        time_out=time.time()
        f=repr(sout.value)
        f1=f[1:]
        f2=f1[:-1]
        execution_time=time_out-time_in
	if (result):
            resultDict = {'value':f2,'execution_time':execution_time*1000}
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            return r
	else:
            resultDict = {'value':'false','execution_time':execution_time*1000}
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            return r
    if request.method == 'GET':
        a=request.args.get('value')
        print a
        b=str(a)
        time_in=time.time()
        sout=ctypes.create_string_buffer(23)
        #lib.SetAgentKeyFromPsw_(pswKey,14)
        result=lib.CipherDecrypt(sout,b)
        time_out=time.time()
        execution_time=time_out-time_in
        if (result):
            resultDict = {'result':str(repr(sout.value)),'execution_time':execution_time}
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            return r
        else:
            resultDict = {'result':'false','execution_time':execution_time}
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            return r



#HomCalculate
@app.route('/todo/api/v1.0/HomCalculateWithEncryption',methods=['POST','GET'])
def HomCalculateWithEncryption():
    if request.method == 'POST':
       type_=request.form['type']
       type_=str(type_)
       first=request.form['first']
       a=str(first)
       #print "a="
       #print a
       second=request.form['second']
       b=str(second)
      # user=request.form['user']
      # user_=str(user)
       user_="zhangsan"
       cur=con.cursor()
       sql="use DemoAuxDB"
       cur.execute(sql)
       con.commit()
       sql1="insert into EncData(Owner,Xdata,Ydata) values("+'\"'+user_+'\"'+','+'\"'+a+'\"'+','+'\"'+b+'\"'+')'
       print sql1
       try:
           cur.execute(sql1)
           con.commit()
       except MySQLdb.Error,e:
           print "Mysql Error %d: %s" % (e.args[0], e.args[1])
       #print "b="
       #print b
       #lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
       if type_=='ADDITION':
            time_in=time.time()
	    sout=ctypes.create_string_buffer(10000)           
            result = lib.AddStringCipher(sout,a,b)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            f=str(repr(sout.value))
            f1=f[1:]
            f2=f1[:-1]
	    if (result):
                resultDict = {'value':f2,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
		resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r

       if type_=='MULTIPLICATION':
            time_in=time.time()
	    sout=ctypes.create_string_buffer(10000)           
            result = lib.MultiStringCipher(sout,a,b)
            time_out=time.time()      
            execution_time=time_out-time_in
            f=str(repr(sout.value))
            f1=f[1:]
            f2=f1[:-1]
            print execution_time
            if (result):
                resultDict = {'value':f2,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
		resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
       if type_=='COMPARE':
            time_in=time.time()
            sout=ctypes.create_string_buffer(10000)
            result = lib.HomComparation(sout,a,b)
            time_out=time.time()
            execution_time=time_out-time_in
            f=str(repr(sout.value))
            f1=f[1:]
            f2=f1[:-1]
            print execution_time
            if (result):
                resultDict = {'value':f2,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
       if type_=='SCALARMULTI':
            #time_in=time.time()
            sout=ctypes.create_string_buffer(10000)
            #sout1=ctypes.create_string_buffer(10000)
            time_in=time.time()
            
            result = lib.MultiScalarStringCipher(sout,b,a)
            
            time_out=time.time()
            execution_time=time_out-time_in
            f=str(repr(sout.value))
            f1=f[1:]
            f2=f1[:-1]
            print execution_time
            if (result):
                resultDict = {'value':f2,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
       if type_=='AND':
            time_in=time.time()
            sout=ctypes.create_string_buffer(10000)
            result = lib.HomAndOperation(sout,a,b)
            time_out=time.time()
            execution_time=time_out-time_in
            f=str(repr(sout.value))
            f1=f[1:]
            f2=f1[:-1]
            print execution_time
            if (result):
                resultDict = {'value':f2,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            
       if type_=='NOT':
            time_in=time.time()
            sout=ctypes.create_string_buffer(10000)
            result = lib.HomNonOperation(sout,a)
            time_out=time.time()
            execution_time=time_out-time_in
            f=str(repr(sout.value))
            f1=f[1:]
            f2=f1[:-1]
            print execution_time
            if (result):
                resultDict = {'value':f2,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
       if type_=='OR':
            time_in=time.time()
            sout=ctypes.create_string_buffer(10000)
            result = lib.HomOrOperation(sout,a,b)
            time_out=time.time()
            execution_time=time_out-time_in
            f=str(repr(sout.value))
            f1=f[1:]
            f2=f1[:-1]
            print execution_time
            if (result):
                resultDict = {'value':f2,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
       if type_=='NAND':
            time_in=time.time()
            sout=ctypes.create_string_buffer(10000)
            result = lib.HomAndNonOperation(sout,a,b)
            time_out=time.time()
            execution_time=time_out-time_in
            f=str(repr(sout.value))
            f1=f[1:]
            f2=f1[:-1]
            print execution_time
            if (result):
                resultDict = {'value':f2,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
       if type_=='XOR':
            time_in=time.time()
            sout=ctypes.create_string_buffer(10000)
            result = lib.HomXorOperation(sout,a,b)
            time_out=time.time()
            execution_time=time_out-time_in
            f=str(repr(sout.value))
            f1=f[1:]
            f2=f1[:-1]
            print execution_time
            if (result):
                resultDict = {'value':f2,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r



"""
    if request.method == 'GET':
       type_=request.args.get('type')
       type_=str(type_)
       first=request.args.get('first')
       a=str(first)
       second=request.args.get('second')
       b=str(second)
       #lib.SetAgentKeyFromPsw_(pswKey,14)
       if type_=='ADDITION':
            time_in=time.time()
            sout=ctypes.create_string_buffer(1000)
            result = lib.AddStringCipher(sout,a,b)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (result):
                resultDict = {'result':repr(sout.value),'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'result':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r

       if type_=='MULTIPLICATION':
            time_in=time.time()
            sout=ctypes.create_string_buffer(1000)
            result = lib.MultiStringCipher(sout,first,second)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (result):
                resultDict = {'result':repr(sout.value),'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'result':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
"""
#HomCalculateWithPlaintext
@app.route('/todo/api/v1.0/HomCalculateWithPlaintext',methods=['POST','GET'])
def HomCalculateWithPlaintext():
    if request.method == 'POST':
        type_=request.form['type']
        type_=str(type_)
        first=request.form['first']
        a=str(first)
        second=request.form['second']
        b=str(second)
        #lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if type_=='ADDITION':
            time_in=time.time()
            sout=ctypes.create_string_buffer(1000)
            result = lib.PlainStringAdd(sout,a,b)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            r=repr(sout.value)
            r=r[1:]
            r=r[:-1]
            if (result):
                resultDict = {'value':r,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r

        if type_=='MULTIPLICATION':
            time_in=time.time()
            sout=ctypes.create_string_buffer(1000)
            result = lib.PlainStringMulti(sout,first,second)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            r=repr(sout.value)
            r=r[1:]
            r=r[:-1]
            if (result):
                resultDict = {'value':r,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
        if type_=='COMPARE':
            time_in=time.time()
            sout=ctypes.create_string_buffer(1000)
            result = lib.HomPlainComparation(sout,first,second)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            r=repr(sout.value)
            r=r[1:]
            r=r[:-1]
            if (result):
                resultDict = {'value':r,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
    
        if type_=='SCALARMULTI':
            time_in=time.time()
            sout=ctypes.create_string_buffer(1000)
            result = lib.PlainStringMulti(sout,first,second)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            r=repr(sout.value)
            r=r[1:]
            r=r[:-1]
            if (result):
                resultDict = {'value':r,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
        if type_=='AND':
            time_in=time.time()
            sout=ctypes.create_string_buffer(1000)
            result = lib.HomPlainAndOperation(sout,first,second)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            r=repr(sout.value)
            r=r[1:]
            r=r[:-1]
            if (result):
                resultDict = {'value':r,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
        if type_=='NOT':
            time_in=time.time()
            sout=ctypes.create_string_buffer(1000)
            result = lib.HomPlainNonOperation(sout,first)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            r=repr(sout.value)
            r=r[1:]
            r=r[:-1]
            if (result):
                resultDict = {'value':r,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
        if type_=='OR':
            time_in=time.time()
            sout=ctypes.create_string_buffer(1000)
            result = lib.HomPlainOrOperation(sout,first,second)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            r=repr(sout.value)
            r=r[1:]
            r=r[:-1]
            if (result):
                resultDict = {'value':r,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
        if type_=='NAND':
            time_in=time.time()
            sout=ctypes.create_string_buffer(1000)
            result = lib.HomPlainAndNonOperation(sout,first,second)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            r=repr(sout.value)
            r=r[1:]
            r=r[:-1]
            if (result):
                resultDict = {'value':r,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
        if type_=='XOR':
            time_in=time.time()
            sout=ctypes.create_string_buffer(1000)
            result = lib.HomPlainXorOperation(sout,first,second)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            r=repr(sout.value)
            r=r[1:]
            r=r[:-1]
            if (result):
                resultDict = {'value':r,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'value':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r

  	   
#HomSingleListEncryption
@app.route('/todo/api/v1.0/HomSingleListEncryption',methods=['POST','GET'])
def HomSingleListEncryption():
    if request.method == 'POST':
        array_single=request.form.getlist('data[]')              
        array_single=str(array_single)      
        da=array_single.replace('u','') 
        g=da.replace('\'','')
        print g
        sout=ctypes.create_string_buffer(10000)
        #b=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if(1):
            time_in=time.time()
            c=lib.PlainVectorEncrypt(sout,g)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (c):
                print c
		print "encryptresult:"
		print repr(sout.value)
                ff=repr(sout.value)
                dp=ff[2:]
                pp=dp[:-2]
                gg=pp.split(',')
                resultDict = {'data':gg,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'data':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
    
#HomSort
@app.route('/todo/api/v1.0/HomSort',methods=['POST','GET'])
def HomSort():
    if request.method == 'POST':
        array_single=request.form.getlist('data[]')
        array_single=str(array_single)
        da=array_single.replace('u','')
        g=da.replace('\'','')
        print g
        sout=ctypes.create_string_buffer(10000)

        #b=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if(1):
            time_in=time.time()
            c=lib.HomSort(sout,g)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (c):
                print c
                print "sortresult:"
                de=repr(sout.value)
                df=de[2:]
                dr=df[:-2]
                dg=dr.split(',')
                print dg
                resultDict = {'data':dg,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'data':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r

#HomSingleListDecryption
@app.route('/todo/api/v1.0/HomSingleListDecryption',methods=['POST','GET'])
def HomSingleListDecryption():
    if request.method == 'POST':    
        array_single=request.form.getlist('data[]')
        array_single=str(array_single)
        da=array_single.replace('u','')
        g=da.replace('\'','')
        print g
        sout=ctypes.create_string_buffer(10000)

        #b=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if(1):
            time_in=time.time()
            c=lib.CipherVectorDecrypt(sout,g)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (c):
                print c
                print "decryptresult:"
                de=repr(sout.value)
                df=de[2:]
                dr=df[:-2]
                dg=dr.split(',')
                print dg
                resultDict = {'data':dg,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'data':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
 
#AveWithEncryption
@app.route('/todo/api/v1.0/AveWithEncryption',methods=['POST','GET'])
def AveWithEncryption():
    if request.method == 'POST':
        array_single=request.form.getlist('data[]')
        array_single=str(array_single)
        da=array_single.replace('u','')
        g=da.replace('\'','')
        print g
        sout=ctypes.create_string_buffer(10000)

        #b=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if(1):
            time_in=time.time()
            c=lib.AveSemiHomoCipherCalculate(sout,g)   
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (c):
                print c
                print "avecalculatecipherresult:"
                ps=repr(sout.value)
                pf=ps[1:]
                pp=pf[:-1]
                print pp
                resultDict = {'result':pp,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'result':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r

#AveWithPlaintext
@app.route('/todo/api/v1.0/AveWithPlaintext',methods=['POST','GET'])
def AveWithPlaintext():
    if request.method == 'POST':
        array_single=request.form.getlist('data[]')
        array_single=str(array_single)
        da=array_single.replace('u','')
        g=da.replace('\'','')
        print g
        sout=ctypes.create_string_buffer(10000)

        #b=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if(1):
            time_in=time.time()
            c=lib.AvePlainCalculate(sout,g)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (c):
                print c
                print "avecalculatecipherresult:"
                ps=repr(sout.value)
                pf=ps[1:]
                pp=pf[:-1]
                print pp
                resultDict = {'result':pp,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'result':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r

#AveWithDecryption
@app.route('/todo/api/v1.0/AveWithDecryption',methods=['POST','GET'])
def AveWithDecryption():
    if request.method == 'POST':
        avecipher=request.form['data']
        avecipher=str(avecipher)
        print avecipher
        sout=ctypes.create_string_buffer(1000)
        #b=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if(1):
            time_in=time.time()
            c=lib.AveSemiHomoCipherDecrypt(sout,avecipher)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (c):
                print c
                print "avedecryptresult:"
                tt=repr(sout.value)
                tr=tt[1:]
                ty=tr[:-1]
                
                print ty
                resultDict = {'result':ty,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'result':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r


#VarWithEncryption
@app.route('/todo/api/v1.0/VarWithEncryption',methods=['POST','GET'])
def VarWithEncryption():
    if request.method == 'POST':
        array_single=request.form.getlist('data[]')
        array_single=str(array_single)
        da=array_single.replace('u','')
        g=da.replace('\'','')
        print g
        sout=ctypes.create_string_buffer(10000)
 
        #b=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if(1):
            time_in=time.time()
            c=lib.VarSemiHomoCipherCalculate(sout,g)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (c):
                print c
                print "varcalculatecipherresult:"
                aa=repr(sout.value)
                ab=aa[1:]
                ac=ab[:-1]
                print ac
                resultDict = {'result':ac,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'result':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r

#STDWithPlaintext
@app.route('/todo/api/v1.0/STDWithPlaintext',methods=['POST','GET'])
def STDWithPlaintext():
    if request.method == 'POST':
        array_single=request.form.getlist('data[]')
        array_single=str(array_single)
        da=array_single.replace('u','')
        g=da.replace('\'','')
        print g
        sout=ctypes.create_string_buffer(10000)

        #b=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if(1):
            time_in=time.time()
            c=lib.VarPlainCalculate(sout,g)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (c):
                print c
                print "avecalculatecipherresult:"
                ps=repr(sout.value)
                pf=ps[1:]
                pp=pf[:-1]
                print pp
                po=float(pp)
                pj=math.sqrt(po)
                resultDict = {'result':str(pj),'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'result':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r




#STDWithDecryption
@app.route('/todo/api/v1.0/STDWithDecryption',methods=['POST','GET'])
def STDWithDecryption():
    if request.method == 'POST':
        varcipher=request.form['data']
        varcipher=str(varcipher)
        print varcipher
        sout=ctypes.create_string_buffer(1000)
        #b=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if(1):
            time_in=time.time()
            c=lib.VarSemiHomoCipherDecrypt(sout,varcipher)
            time_out=time.time()
            execution_time=time_out-time_in
            s=repr(sout.value)
            s1=s[1:]
            s2=s1[:-1]
            m=float(s2)
            e=math.sqrt(m)
            print execution_time
            if (c):
                print c
                print "STDdecryptresult:"
                print e
                resultDict = {'result':str(e),'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'result':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
#HomDoubleListEncryption
@app.route('/todo/api/v1.0/HomDoubleListEncryption',methods=['POST','GET'])
def HomDoubleListEncryption():
    if request.method == 'POST':
        data1=request.form.getlist('data[]')
        data1=str(data1)
        data2=data1.replace('u\'{"x":','(')
        data3=data2.replace('"y":','')
        data4=data3.replace('}\'',')')  
        print data4
        sout=ctypes.create_string_buffer(10000)
        #b=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if(1):
            time_in=time.time()
            c=lib.PlainVectorPairEncrypt(sout,data4)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (c):
                print c
                print "doublelistencryptresult:"
                p=repr(sout.value)
                p1=p[2:]
                p2=p1[:-2]
                p3=p2.replace('(','{')
                p4=p3.replace(')','}')
                p5=p4.replace(', ',' ')
                p6=p5.replace('} ','},')
                p7=p6.replace('{','{\'x\':')
                p8=p7.replace(' ',',\'y\':')
                p9=p8.replace('},','}, ')                      
                p10=p9.split(', ')
                for i in range(len(p10)):
                    p10[i]=p10[i].replace('\'x\':','\'x\':\'')
                    p10[i]=p10[i].replace(',\'y\':','\',\'y\':\'')                 
                    p10[i]=p10[i].replace('}','\'}')
                    p10[i]=eval(p10[i])
                    
                    print type(p10[i])
                
                resultDict = {'data':p10,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'result':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r

#HomDoubleListDecryption
@app.route('/todo/api/v1.0/HomDoubleListDecryption',methods=['POST','GET'])
def HomDoubleListDecryption():
     if request.method == 'POST':
        data1=request.form.getlist('data[]')
        data1=str(data1)
        data2=data1.replace('u\'{"x":','(')
        data3=data2.replace('"y":','')
        data4=data3.replace('}\'',')')
        data5=data4.replace('\"','')
        print data5

        sout=ctypes.create_string_buffer(10000)
        #b=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if(1):
            time_in=time.time()
            c=lib.CipherVectorPairDecrypt(sout,data5)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (c):
                print c
                print "doublelistdecryptresult:"
                p=repr(sout.value)
                p1=p[2:]
                p2=p1[:-2]
                p3=p2.replace('(','{')
                p4=p3.replace(')','}')
                p5=p4.replace(', ',' ')
                p6=p5.replace('} ','},')
                p7=p6.replace('{','{\'x\':')
                p8=p7.replace(' ',',\'y\':')
                p9=p8.replace('},','}, ')
                p10=p9.split(', ')
                for i in range(len(p10)):
                    p10[i]=p10[i].replace('\'x\':','\'x\':\'')
                    p10[i]=p10[i].replace(',\'y\':','\',\'y\':\'')              
                    p10[i]=p10[i].replace('}','\'}')
                    p10[i]=eval(p10[i])

                    print type(p10[i])

                resultDict = {'data':p10,'execution_time':execution_time*1000}

                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'result':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
   
#RegressionWithEncryption
@app.route('/todo/api/v1.0/RegressionWithEncryption',methods=['POST','GET'])
def RegressionWithEncryption():
     if request.method == 'POST':
        data1=request.form.getlist('data[]')
        data1=str(data1)
        data2=data1.replace('u\'{"x":','(')
        data3=data2.replace('"y":','')
        data4=data3.replace('}\'',')')
        data5=data4.replace('\"','')

        print data5

        s1=ctypes.create_string_buffer(1000)
        s2=ctypes.create_string_buffer(1000)
        #b=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if(1):
            time_in=time.time()
            c=lib.LinearRegressionSemiHomoCipherCalculate(s1,s2,data5)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (c):
                print c
                print "LinearRegressionSemiHomoCipherCalculateresult:"
                print repr(s1.value)
                print repr(s2.value)
                resultDict = {'slope':repr(s1.value),'intercept':repr(s2.value),'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'slope':'false','intercept':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
#SlopeWithDecryption
@app.route('/todo/api/v1.0/SlopeWithDecryption',methods=['POST','GET'])
def SlopeWithDecryption():
    if request.method == 'POST':
        slopecipher=request.form['data']
        slopecipher=str(slopecipher)
        print slopecipher
        slopecipher=slopecipher[1:]
        slopecipher=slopecipher[:-1]
        sout=ctypes.create_string_buffer(1000)
        #b=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if(1):
            time_in=time.time()
            c=lib.SlopeCipherDecrypt(sout,slopecipher)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (c):
                print c
                print "slopedecryptresult:"
                rr= repr(sout.value)
                print rr
                rr=rr[:-1]
                rr=rr[1:]
                rr=float(rr)
               
                resultDict = {'result':rr,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'result':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r

#InterceptWithDecryption
@app.route('/todo/api/v1.0/InterceptWithDecryption',methods=['POST','GET'])
def InterceptWithDecryption():
    if request.method == 'POST':
        interceptcipher=request.form['data']
        interceptcipher=str(interceptcipher)
        interceptcipher=interceptcipher[1:]
        interceptcipher=interceptcipher[:-1]
        print interceptcipher
        sout=ctypes.create_string_buffer(1000)
        #b=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if(1):
            time_in=time.time()
            c=lib.InterceptCipherDecrypt(sout,interceptcipher)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (c):
                print c
                print "Interceptdecryptresult:"
                rr= repr(sout.value)
                print rr
                rr=rr[:-1]
                rr=rr[1:]
                rr=float(rr)
                resultDict = {'result':rr,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'result':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
        else:
           resultDict = {'result':'SetAgentKeyFromPsw false','execution_time':execution_time*1000}
           r = make_response(jsonify(resultDict))
           r.headers["Access-Control-Allow-Origin"] =  "*"
           return r
   
#RegressionWithPlaintext
@app.route('/todo/api/v1.0/RegressionWithPlaintext',methods=['POST','GET'])
def RegressionWithPlaintext():
    
     if request.method == 'POST':
        data1=request.form.getlist('data[]')
        data1=str(data1)
        data2=data1.replace('u\'{"x":','(')
        data3=data2.replace('"y":','')
        data4=data3.replace('}\'',')')
        data5=data4.replace('\"','')

        print data5

        s1=ctypes.create_string_buffer(1000)
        s2=ctypes.create_string_buffer(1000)
        # b=lib.SetAgentKeyFromPsw_(pswKey['foo'],14)
        if(1):
            time_in=time.time()
            c=lib.LinearRegressionSemiHomoPlainCalculate(s1,s2,data5)
            time_out=time.time()
            execution_time=time_out-time_in
            print execution_time
            if (c):
                print c
                print "LinearRegressionSemiHomoPlainCalculateresult:"
	        p=repr(s1.value)
                q=repr(s2.value)
                print p
                print q
                p=p[1:]
                p=p[:-1]
                p=float(p)
                q=q[1:]
                q=q[:-1]
                q=float(q)
                resultDict = {'slope':p,'intercept':q,'execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
            else:
                resultDict = {'slope':'false','intercept':'false','execution_time':execution_time*1000}
                r = make_response(jsonify(resultDict))
                r.headers["Access-Control-Allow-Origin"] =  "*"
                return r
        else:
            resultDict = {'slope':'SetAgentKeyFromPsw false','intercept':'SetAgentKeyFromPsw false','execution_time':execution_time*1000}
            r = make_response(jsonify(resultDict))
            r.headers["Access-Control-Allow-Origin"] =  "*"
            return r  
      

if __name__ == '__main__':
     app.run(debug=True,host='0.0.0.0')
