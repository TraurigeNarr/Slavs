//gsoap ns service name:	slavsserver
//gsoap ns service style:	rpc
//gsoap ns service encoding:	encoded
//gsoap ns service namespace:	urn:slavsserver
//gsoap ns service location:	http://localhost:9999
//gsoap ns service method-action: MakePayment urn:MakePayment

typedef int xsd__int;
typedef char* xsd__string;

enum t__status				// remote status:
{ 
  STATE_OK,					// ok
  STATE_FAIL				// fail to process
};

class t__result
{
public:
  enum t__status errCode;
  xsd__string message;
};

int ns__MakePayment(xsd__int qty, t__result* result);

//gsoap ns service name:	slavsserver
//gsoap ns service style:	document
//gsoap ns service encoding:	literal
//gsoap ns service namespace:	urn:slavsserver
//gsoap ns service location:	http://localhost:9999
//gsoap ns service method-action: MakePayment urn:MakePayment
int ns__OneFunction(xsd__string some, t__result* result);
