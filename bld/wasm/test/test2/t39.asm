.model small
.data

	dw '1'
	dw '12'
	dd '1'
	dd '12'
	dd '123'
	dd '1234'
	dq '1'
	dq '12'
	dq '123'
	dq '1234'
	dq '12345'
	dq '123456'
	dq '1234567'
	dq '12345678'
ifdef errors
	dw '123'
	dd '12345'
	dq '123456789'
endif

end
