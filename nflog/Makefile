binary	:=nflog
$(binary):%:%.c
	gcc $^ -lnetfilter_log -o $@
clean:
	-rm -rf $(binary)
.PHONY:
	clean
