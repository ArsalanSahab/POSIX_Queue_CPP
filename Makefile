clean_compile : posix_publisher.cpp posix_consumer.cpp

		echo "Cleaning Directory.."
		rm -f *.o posix_consumer posix_publisher
		echo "Done.."

		echo "Compiling..."
		g++ -c -Wall -g posix_consumer.cpp
		g++ -c -Wall -g posix_publisher.cpp
		g++ posix_publisher.o -o posix_publisher -lrt -lpthread
		g++ posix_consumer.o -o posix_consumer -lrt -lpthread
		echo "Done.."
