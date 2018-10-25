test : libSemiHomo.so main.cpp SemiHomoFun.h ThresholdPaillierAgent.h ThresholdPaillierServer.h PaillierPrivate.h PaillierPublic.h
	g++ main.cpp -L. -o test -lSemiHomo -lntl -lcrypto
main : obj/main.o obj/SemiHomoFun.o obj/ThresholdPaillierAgent.o obj/ThresholdPaillierServer.o obj/PaillierPrivate.o obj/PaillierPublic.o
	g++ obj/main.o obj/SemiHomoFun.o obj/ThresholdPaillierAgent.o obj/ThresholdPaillierServer.o obj/PaillierPrivate.o obj/PaillierPublic.o -o main -lntl -lcrypto
libSemiHomo.so : SemiHomoFun.h ThresholdPaillierAgent.h ThresholdPaillierServer.h PaillierPrivate.h PaillierPublic.h SemiHomoFun.cpp ThresholdPaillierAgent.cpp ThresholdPaillierServer.cpp PaillierPrivate.cpp PaillierPublic.cpp
	g++ SemiHomoFun.cpp ThresholdPaillierAgent.cpp ThresholdPaillierServer.cpp PaillierPrivate.cpp PaillierPublic.cpp -fPIC -shared -o libSemiHomo.so
obj/main.o : main.cpp SemiHomoFun.h ThresholdPaillierAgent.h ThresholdPaillierServer.h PaillierPrivate.h PaillierPublic.h
	g++ -c main.cpp -o obj/main.o
obj/SemiHomoFun.o : SemiHomoFun.h ThresholdPaillierAgent.h ThresholdPaillierServer.h PaillierPrivate.h PaillierPublic.h
	g++ -c SemiHomoFun.cpp -o obj/SemiHomoFun.o
obj/ThresholdPaillierAgent.o : ThresholdPaillierAgent.h PaillierPrivate.h PaillierPublic.h
	g++ -c ThresholdPaillierAgent.cpp -o obj/ThresholdPaillierAgent.o
obj/ThresholdPaillierServer.o : ThresholdPaillierServer.h PaillierPublic.h
	g++ -c ThresholdPaillierServer.cpp -o obj/ThresholdPaillierServer.o
obj/PaillierPrivate.o : PaillierPrivate.cpp PaillierPrivate.h PaillierPublic.h
	g++ -c PaillierPrivate.cpp -o obj/PaillierPrivate.o
obj/PaillierPublic.o : PaillierPublic.cpp PaillierPublic.h
	g++ -c PaillierPublic.cpp -o obj/PaillierPublic.o
clean : 
	rm obj/*.o
