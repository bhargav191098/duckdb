#include "duckdb/common/types/row/tuple_data_segment.hpp"

namespace duckdb {

TupleDataChunkPart::TupleDataChunkPart() {
}

TupleDataChunk::TupleDataChunk() : count(0) {
}

static inline void SwapTupleDataChunk(TupleDataChunk &a, TupleDataChunk &b) noexcept {
	std::swap(a.parts, b.parts);
	std::swap(a.row_block_ids, b.row_block_ids);
	std::swap(a.heap_block_ids, b.heap_block_ids);
	std::swap(a.count, b.count);
}

TupleDataChunk::TupleDataChunk(TupleDataChunk &&other) noexcept {
	SwapTupleDataChunk(*this, other);
}

TupleDataChunk &TupleDataChunk::operator=(TupleDataChunk &&other) noexcept {
	SwapTupleDataChunk(*this, other);
	return *this;
}

void TupleDataChunk::AddPart(TupleDataChunkPart &&part) {
	count += part.count;
	row_block_ids.insert(part.row_block_index);
	heap_block_ids.insert(part.heap_block_index);
	parts.emplace_back(std::move(part));
}

void TupleDataChunk::Verify() const {
#ifdef DEBUG
	idx_t total_count = 0;
	for (const auto &part : parts) {
		total_count += part.count;
	}
	D_ASSERT(this->count == total_count);
	D_ASSERT(this->count <= STANDARD_VECTOR_SIZE);
#endif
}

void TupleDataChunk::MergeLastChunkPart() {
	if (parts.size() < 2) {
		return;
	}

	auto &second_to_last_part = parts[parts.size() - 2];
	auto &last_part = parts[parts.size() - 1];
	if (last_part.row_block_index == second_to_last_part.row_block_index &&
	    last_part.heap_block_index == second_to_last_part.heap_block_index &&
	    last_part.base_heap_ptr == second_to_last_part.base_heap_ptr) {
		// These parts have the same row and heap blocks - merge them
		second_to_last_part.total_heap_size += last_part.total_heap_size;
		second_to_last_part.count += last_part.count;
		parts.pop_back();
	}
}

TupleDataSegment::TupleDataSegment(shared_ptr<TupleDataAllocator> allocator_p)
    : allocator(std::move(allocator_p)), count(0) {
}

void SwapTupleDataSegment(TupleDataSegment &a, TupleDataSegment &b) {
	std::swap(a.allocator, b.allocator);
	std::swap(a.chunks, b.chunks);
	std::swap(a.count, b.count);
	std::swap(a.pinned_handles, b.pinned_handles);
}

TupleDataSegment::TupleDataSegment(TupleDataSegment &&other) noexcept {
	SwapTupleDataSegment(*this, other);
}

TupleDataSegment &TupleDataSegment::operator=(TupleDataSegment &&other) noexcept {
	SwapTupleDataSegment(*this, other);
	return *this;
}

idx_t TupleDataSegment::ChunkCount() const {
	return chunks.size();
}

idx_t TupleDataSegment::SizeInBytes() const {
	const auto &layout = allocator->GetLayout();
	idx_t total_size = 0;
	for (const auto &chunk : chunks) {
		total_size += chunk.count * layout.GetRowWidth();
		if (!layout.AllConstant()) {
			for (const auto &part : chunk.parts) {
				total_size += part.total_heap_size;
			}
		}
	}
	return total_size;
}

void TupleDataSegment::Combine(TupleDataSegment &other) {
	D_ASSERT(allocator.get() == other.allocator.get());

	// Merge the chunks in order - should slightly improve sequential scans
	const idx_t total_chunks = this->chunks.size() + other.chunks.size();
	vector<TupleDataChunk> combined_chunks;
	combined_chunks.reserve(total_chunks);

	auto this_it = this->chunks.begin();
	auto other_it = other.chunks.begin();
	while (combined_chunks.size() != total_chunks) {
		if (this_it == chunks.end()) {
			combined_chunks.push_back(std::move(*other_it));
			other_it++;
		} else if (other_it == other.chunks.end()) {
			combined_chunks.push_back(std::move(*this_it));
			this_it++;
		} else if (this_it->parts[0].row_block_index < other_it->parts[0].row_block_index) {
			combined_chunks.push_back(std::move(*this_it));
			this_it++;
		} else {
			combined_chunks.push_back(std::move(*other_it));
			other_it++;
		}
	}
	other.chunks.clear();
	other.count = 0;

	this->chunks = std::move(combined_chunks);
	this->count += other.count;

	this->pinned_handles.reserve(this->pinned_handles.size() + other.pinned_handles.size());
	for (auto &other_pinned_handle : other.pinned_handles) {
		this->pinned_handles.push_back(std::move(other_pinned_handle));
	}
	other.pinned_handles.clear();
}

void TupleDataSegment::Verify() const {
#ifdef DEBUG
	idx_t total_count = 0;
	for (const auto &chunk : chunks) {
		chunk.Verify();
		total_count += chunk.count;
	}
	D_ASSERT(total_count == this->count);
#endif
}

} // namespace duckdb
