ISA_OUT_DIR:=$(CWD)/$(GENERATED)
ISA_OUT_LIB:=$(CWD)/$(GENERATED)/lib

ISA_L_OUT:=$(ISA_OUT_LIB)/libisal_crypto.a
ISA_L_DIR:=third-party/isa-l_crypto

.PHONY: isa_l

isa_l: $(ISA_OUT_LIB)/libisal_crypto.a

$(ISA_OUT_LIB)/libisal_crypto.a: 
	@printf "Making $(ISA_L_OUT)\n"
	@(cd $(ISA_L_DIR) && ./autogen.sh && ./configure --prefix=$(ISA_OUT_DIR) --libdir=$(ISA_OUT_LIB) && make && make install)



