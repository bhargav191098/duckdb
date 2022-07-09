#include "duckdb_python/exceptions.hpp"

#include "duckdb/common/exception.hpp"
#include "duckdb_python/pybind_wrapper.hpp"

namespace py = pybind11;

namespace duckdb {

/**
 * @see https://peps.python.org/pep-0249/#exceptions
 */
void RegisterExceptions(const py::module m) {
	// This is the error structure defined in the DBAPI spec
	// StandardError
	// |__ Warning
	// |__ Error
	//    |__ InterfaceError
	//    |__ DatabaseError
	//       |__ DataError
	//       |__ OperationalError
	//       |__ IntegrityError
	//       |__ InternalError
	//       |__ ProgrammingError
	//       |__ NotSupportedError
	// The base class is mapped to Error in python to somewhat match the DBAPI 2.0 specifications
	auto errorClass = py::register_exception<Exception>(m, "Error").ptr();

	py::register_exception<NotImplementedException>(m, "NotSupportedError", errorClass);

	py::register_exception<BinderException>(m, "BinderException", errorClass);
	py::register_exception<CastException>(m, "CastException", errorClass);
	py::register_exception<CatalogException>(m, "CatalogException", errorClass);
	py::register_exception<ConstraintException>(m, "ConstraintException", errorClass);
	py::register_exception<ConversionException>(m, "ConversionException", errorClass);
	py::register_exception<FatalException>(m, "FatalException", errorClass);
	py::register_exception<InternalException>(m, "InternalException", errorClass);
	py::register_exception<InterruptException>(m, "InterruptException", errorClass);
	py::register_exception<InvalidInputException>(m, "InvalidInputException", errorClass);
	py::register_exception<InvalidTypeException>(m, "InvalidTypeException", errorClass);
	py::register_exception<IOException>(m, "IOException", errorClass);
	py::register_exception<OutOfMemoryException>(m, "OutOfMemoryException", errorClass);
	py::register_exception<OutOfRangeException>(m, "OutOfRangeException", errorClass);
	py::register_exception<ParserException>(m, "ParserException", errorClass);
	py::register_exception<PermissionException>(m, "PermissionException", errorClass);
	py::register_exception<SequenceException>(m, "SequenceException", errorClass);
	py::register_exception<SerializationException>(m, "SerializationException", errorClass);
	py::register_exception<SyntaxException>(m, "SyntaxException", errorClass);
	py::register_exception<TransactionException>(m, "TransactionException", errorClass);
	py::register_exception<TypeMismatchException>(m, "TypeMismatchException", errorClass);
	py::register_exception<ValueOutOfRangeException>(m, "ValueOutOfRangeException", errorClass);

	py::register_exception<StandardException>(m, "StandardException", errorClass);
}
} // namespace duckdb
