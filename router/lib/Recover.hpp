#ifndef RECOVER_HPP_
#define RECOVER_HPP_

#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <string_view>

namespace storage::utils {
constexpr inline std::size_t g_disk_size = 5024;
// Значения по умолчанию для нашей конфигурации серверов
template <typename std::size_t BlockSize = 4, 
		  typename std::size_t DiskNum = 2>
class FileRecover final {
	using file_storage = std::vector<std::string>;

public:
	explicit FileRecover(std::string_view buffer) {
		m_file = write_strip_data(buffer);
		std::for_each(m_file.begin(), m_file.end(), [](auto& str) { 
			str.resize(g_disk_size); 
		});
		m_paritet.resize(g_disk_size);
		init_parity();
	}

	explicit FileRecover(const std::vector<std::string>& blocks) {
		m_file = blocks;
	}

	explicit FileRecover(const std::string& buffer,
						 std::size_t disk_num, std::size_t to_disk,
						 const std::vector<std::byte>& paritet) {
		m_file = file_storage(DiskNum);
		m_file[disk_num] = buffer;
		std::for_each(m_file.begin(), m_file.end(), [](auto& str) {
			str.resize(g_disk_size);
		});
		m_paritet = paritet;
		raid_recover(disk_num, to_disk);
	}

	FileRecover(const FileRecover&) = delete;

	FileRecover(FileRecover&&) = delete;

public:
	FileRecover& operator=(const FileRecover&) = delete;

	FileRecover& operator=(FileRecover&&) = delete;

public:
	// Разбивает данные на блоки и хранит в массиве строк
	[[nodiscard]] file_storage write_strip_data(std::string_view buffer) const {
		file_storage disks(DiskNum);

		for (std::size_t i = 0; i < buffer.size(); i += BlockSize) {
			std::size_t disk_id = (i / BlockSize) % DiskNum;
			disks[disk_id].append(buffer.data() + i, BlockSize);
		}

		return disks;
	}

	// Читает данные разделенные блоками и воспроизводит в изначальный порядок
	[[nodiscard]] std::string read_strip_data() {
		std::string buffer;
		int ind = get_min_size_ind();
		int i = 0;
		for (; i < m_file[ind].size(); i += BlockSize) {
			std::vector<std::string> tmp(DiskNum);
			for (int k = 0; k < DiskNum; k++) {
				if (m_file[k].size() % BlockSize == 0) {
					for (int j = i; j < i + BlockSize; j++) {
						tmp[k].push_back(m_file[k][j]);
					}
					buffer.append(tmp[k]);
				} else {
					for (int j = i; j < m_file[k].size(); j++) {
						tmp[k].push_back(m_file[k][j]);
					}
					buffer.append(tmp[k]);
				}
			}
		}
		if (0 != ind) {
			for (; i < m_file.front().size(); i += BlockSize) {
				std::vector<std::string> tmp(ind);
				for (int k = 0; k < ind; k++) {
					if (m_file[k].size() % BlockSize == 0) {
						for (int j = i; j < i + BlockSize; j++) {
							tmp[k].push_back(m_file[k][j]);
						}
						buffer.append(tmp[k]);
					} else {
						for (int j = i; j < m_file[k].size(); j++) {
							tmp[k].push_back(m_file[k][j]);
						}
						buffer.append(tmp[k]);
					}
				}
			}
		}

		return buffer;
	}
	
	void init_parity() {
		for (auto& str : m_file) {
			for (std::size_t i = 0; i < g_disk_size; i++) {
				m_paritet[i] = compute_parity(static_cast<std::byte>(m_file[0][i]), 
					static_cast<std::byte>(m_file[1][i]));
			}
		}
	}

	[[nodiscard]] void raid_recover(std::size_t disk_from, std::size_t disk_error) {
		if (disk_from > m_file.size() || disk_error > m_file.size()) {
			throw std::runtime_error("disk number error.");
		}
		
		std::string restored_data(g_disk_size, ' ');
		for (auto& str : m_file) {
			for (std::size_t i = 0; i < g_disk_size; i++) {
				restored_data[i] = static_cast<char>(recover_data(static_cast<std::byte>(m_file[disk_from][i]),
					static_cast<std::byte>(m_paritet[i])));
			}
		}

		m_file[disk_error] = std::move(restored_data);
	}

	[[nodiscard]] constexpr file_storage& get_strip_file() noexcept { return m_file; }

	[[nodiscard]] constexpr const std::vector<std::byte>& get_parity_data() const noexcept { return m_paritet; }

private:
	// Функция для вычисления паритета
	[[nodiscard]] constexpr std::byte compute_parity(std::byte a, std::byte b) const noexcept {
		return a ^ b;
	}

	// Функция для восстановления потерянных данных
	[[nodiscard]] constexpr std::byte recover_data(std::byte a, std::byte parity) const noexcept {
		return a ^ parity;
	}

	[[nodiscard]] constexpr std::size_t get_min_size_ind() const {
		std::size_t min = INT32_MAX;
		std::size_t min_ind = 0;
		for (std::size_t i = 0; i < m_file.size(); i++) {
			if (m_file[i].size() < min) {
				min = m_file[i].size();
				min_ind = i;
			}
		}
		return min_ind;
	}

private:
	std::vector<std::byte> m_paritet;
	file_storage m_file;
};
}

#endif
