TARGET	= ggsample15
SOURCES	= $(wildcard *.cpp)
HEADERS	= $(wildcard *.h)
OBJECTS	= $(patsubst %.cpp,%.o,$(SOURCES))
CXXFLAGS	= --std=c++17 -pthread -g -Wall -DDEBUG -DX11 -DPROJECT_NAME=\"$(TARGET)\" `pkg-config glfw3  --cflags`
LDLIBS	= -ldl `pkg-config glfw3 --libs`

.PHONY: clean

$(TARGET): $(OBJECTS)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(TARGET).dep: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -MM $(SOURCES) > $@

clean:
	-$(RM) $(TARGET) *.o lib/*.o *~ .*~ *.bak *.dep imgui.ini a.out core

-include $(TARGET).dep
