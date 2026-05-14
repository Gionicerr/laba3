# Названия итоговых файлов
TARGET_APP = main
TARGET_TEST = test

# Компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Флаги для подключения Google Test
# -lgtest: основная библиотека
# -lgtest_main: стандартная функция main для тестов
# -lpthread: необходим для работы gtest в системах MinGW/MSYS2
GTEST_FLAGS = -lgtest -lgtest_main -lpthread

# Список всех заголовочных файлов для отслеживания изменений
DEPS = Sequence.h ArraySequence.h MutableArraySequence.h \
       ImmutableArraySequence.h ListSequence.h DynamicArray.h \
       LinkedList.h Exceptions.h

# Объектные файлы для основной программы
OBJ_APP = main.o

# Объектные файлы для тестов
OBJ_TEST = TestRun.o TestListSeq.o TestLinkList.o TestDynArr.o TestArrSeq.o

# Цель по умолчанию — собирает обе программы
all: $(TARGET_APP) $(TARGET_TEST)

# Сборка основной программы
$(TARGET_APP): $(OBJ_APP)
	$(CXX) -o $@ $^ $(CXXFLAGS)

# Сборка программы с тестами (добавлены флаги линковки gtest)
$(TARGET_TEST): $(OBJ_TEST)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(GTEST_FLAGS)

# Общее правило для компиляции .cpp в .o
%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# Отдельная цель для запуска тестов
run_tests: $(TARGET_TEST)
	./$(TARGET_TEST)

# Очистка мусора
clean:
	rm -f *.o $(TARGET_APP) $(TARGET_TEST)

.PHONY: all clean run_tests