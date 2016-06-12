# These can be modified.
NAME     := hikisetmiehet.exe
SRCDIR   := src
OBJDIR   := obj
BINDIR   := bin
DEPDIR   := dep

# These can be modified too.
CXX      := g++
CPPFLAGS := -DGLFW_INCLUDE_GLCOREARB
CXXFLAGS := -g -O0 -Wall -Wextra -Wshadow -pedantic
LDLIBS   := -lGL -lglfw -lsfml-audio
INCL     := -Iinclude

# These should not be modified.
BIN  := $(BINDIR)/$(NAME)
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEPS := $(SRCS:$(SRCDIR)/%.cpp=$(DEPDIR)/%.d)
CPPFLAGS += -MMD -MP

.PHONY: clean run


# Default target.
$(BIN): $(OBJS) | $(BINDIR)
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

# Handle dependencies.
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(INCL) -MT $@ -MF $(DEPDIR)/$*.d $(CXXFLAGS) -c -o $@ $<

# Create the necessary directories if they don't exist.
$(OBJS): | $(OBJDIR) $(DEPDIR)

$(BINDIR) $(OBJDIR) $(DEPDIR):
	mkdir $@

clean:
	$(RM) $(BIN) $(OBJS) $(DEPS)
	rmdir $(BINDIR) $(OBJDIR) $(DEPDIR)

run: $(BIN)
	$(BINDIR)/$(NAME)

-include $(DEPS)
