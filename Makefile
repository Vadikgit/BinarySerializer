CODEGEN := codegen
TEST_BUILD := test/build

ALL_LDFLAGS := $(LDFLAGS) $(EXTRA_LDFLAGS)

clean:
	@rm -rf $(CODEGEN)
	@rm -rf $(TEST_BUILD)

# Directory
$(CODEGEN):
	$(Q)mkdir $@

$(CODEGEN)/codegen: BinarySerializer.cpp types.h | $(CODEGEN)
	@$(CXX) $(CXXFLAGS) -std=c++20 -g BinarySerializer.cpp -o $@

$(TEST_BUILD):
	$(Q)mkdir $@
	
$(TEST_BUILD)/tester: $(CODEGEN)/codegen | $(TEST_BUILD)
	cp types.h test/
	cd test; \
	../$(CODEGEN)/codegen; \
	$(CXX) $(CXXFLAGS) -g test.cpp tester.cpp -o build/tester

complete: $(CODEGEN)/codegen $(TEST_BUILD)/tester

.DELETE_ON_ERROR:

.SECONDARY:
