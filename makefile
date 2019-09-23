all:main1 main2 main3 main4 main5 main6 main7

main1:main1.cpp lib/comgraph.h basic_calc_pack/basic_calc_pack.h advanced_calc_pack/advanced_calc_pack.h compare_calc_pack/compare_calc_pack.h
	g++ -o  main1 main1.cpp -std=c++11

main2:main2.cpp lib/comgraph.h basic_calc_pack/basic_calc_pack.h advanced_calc_pack/advanced_calc_pack.h compare_calc_pack/compare_calc_pack.h
	g++ -o main2 main2.cpp -std=c++11

main3:main3.cpp lib/comgraph.h basic_calc_pack/basic_calc_pack.h advanced_calc_pack/advanced_calc_pack.h compare_calc_pack/compare_calc_pack.h
	g++ -o main3 main3.cpp -std=c++11

main4:main4.cpp lib/comgraph.h basic_calc_pack/basic_calc_pack.h advanced_calc_pack/advanced_calc_pack.h compare_calc_pack/compare_calc_pack.h
	g++ -o main4  main4.cpp -std=c++11

main5:main5.cpp lib/comgraph.h basic_calc_pack/basic_calc_pack.h advanced_calc_pack/advanced_calc_pack.h compare_calc_pack/compare_calc_pack.h
	g++ -o main5  main5.cpp -std=c++11

main6:main6.cpp lib/comgraph.h basic_calc_pack/basic_calc_pack.h advanced_calc_pack/advanced_calc_pack.h compare_calc_pack/compare_calc_pack.h
	g++ -o main6  main6.cpp -std=c++11

main7:main7.cpp lib/comgraph.h basic_calc_pack/basic_calc_pack.h advanced_calc_pack/advanced_calc_pack.h compare_calc_pack/compare_calc_pack.h
	g++ -o main7  main7.cpp -std=c++11
