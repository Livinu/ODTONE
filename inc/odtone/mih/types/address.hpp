//=============================================================================
// Brief   : MIH Address Types
// Authors : Bruno Santos <bsantos@av.it.pt>
//------------------------------------------------------------------------------
// ODTONE - Open Dot Twenty One
//
// Copyright (C) 2009-2011 Universidade Aveiro
// Copyright (C) 2009-2011 Instituto de Telecomunicações - Pólo Aveiro
//
// This software is distributed under a license. The full license
// agreement can be found in the file LICENSE in this distribution.
// This software may not be copied, modified, sold or distributed
// other than expressed in the named license agreement.
//
// This software is distributed without any warranty.
//==============================================================================

#ifndef ODTONE_MIH_TYPES_ADDRESS__HPP_
#define ODTONE_MIH_TYPES_ADDRESS__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <odtone/mih/types/base.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace odtone { namespace mih {

///////////////////////////////////////////////////////////////////////////////
/**
 * Define CELL_ID data type.
 */
typedef uint32   cell_id;

/**
 * Define LAC data type.
 */
typedef uint16   lac;

/**
 * Define CI data type.
 */
typedef uint16   ci;

///////////////////////////////////////////////////////////////////////////////
/**
 * Define TRANSPORT_ADDR data type.
 */
class transport_addr {
protected:
	transport_addr(uint16 type) : _type(type)
	{ }

	transport_addr(uint16 type, const void* raw, size_t len)
		: _type(type), _addr(reinterpret_cast<const char*>(raw), len)
	{ }

public:
	uint16 type() const { return _type; }

	const void* get() const    { return _addr.data(); }
	size_t      length() const { return _addr.length(); }

	template<class ArchiveT>
	void serialize(ArchiveT& ar)
	{
		ar & _type;
		ar & _addr;
	}

protected:
	uint16       _type;
	octet_string _addr;
};

///////////////////////////////////////////////////////////////////////////////
/**
 * The enumeration of TRANSPORT_TYPE data type.
 */
enum transport_type_enum {
	l2           = 0,
	l3_or_higher = 1,
};

/**
 * Define TRANSPORT_TYPE data type.
 */
typedef enumeration<transport_type_enum> transport_type;

///////////////////////////////////////////////////////////////////////////////
/**
 * Define MAC_ADDR data type.
 */
class mac_addr : public transport_addr {
public:
	mac_addr() : transport_addr(6)
	{ }

	explicit mac_addr(const octet_string& addr) : transport_addr(6)
	{
		this->address(addr);
	}

	mac_addr(const void* raw, size_t len) : transport_addr(6, raw, len)
	{ }

	octet_string address() const;
	void         address(const octet_string& addr);

	friend std::ostream& operator<<(std::ostream& out, const mac_addr& tp)
	{
		out << "\ntype: " << tp.type();
		out << "\naddress: " << tp.address();

		return out;
	}

	bool operator==(const mac_addr& other) const
	{
		return ((type() == other.type()) && (address().compare(other.address()) == 0));
	}
};

///////////////////////////////////////////////////////////////////////////////
/**
 * Define L2_3GPP_2G_CELL_ID data type.
 */
struct l2_3gpp_2g_cell_id {
	uint8  plmn_id[3];
	lac    _lac;
	ci     _ci;

	template<class ArchiveT>
	void serialize(ArchiveT& ar)
	{
		ar & plmn_id[0];
		ar & plmn_id[1];
		ar & plmn_id[2];
		ar & _lac;
		ar & _ci;
	}

	friend std::ostream& operator<<(std::ostream& out, const l2_3gpp_2g_cell_id&)
	{
		return out;
	}

	bool operator==(const l2_3gpp_2g_cell_id& other) const
	{
		return ((_lac == other._lac)
				&& (_ci == other._ci)
				&& (plmn_id[0] == other.plmn_id[0])
				&& (plmn_id[1] == other.plmn_id[1])
				&& (plmn_id[2] == other.plmn_id[2]));
	}
};

/**
 * Define L2_3GPP_3G_CELL_ID data type.
 */
struct l2_3gpp_3g_cell_id {
	uint8    plmn_id[3];
	cell_id  _cell_id;

	template<class ArchiveT>
	void serialize(ArchiveT& ar)
	{
		ar & plmn_id[0];
		ar & plmn_id[1];
		ar & plmn_id[2];
		ar & _cell_id;
	}

	friend std::ostream& operator<<(std::ostream& out, const l2_3gpp_3g_cell_id&)
	{
		return out;
	}

	bool operator==(const l2_3gpp_3g_cell_id& other) const
	{
		return ((_cell_id == other._cell_id)
				&& (plmn_id[0] == other.plmn_id[0])
				&& (plmn_id[1] == other.plmn_id[1])
				&& (plmn_id[2] == other.plmn_id[2]));
	}
};

///////////////////////////////////////////////////////////////////////////////
/**
 * Define L2_3GPP_ADDR data type.
 */
struct l2_3gpp_addr  {
	template<class ArchiveT>
	void serialize(ArchiveT& ar)
	{
		ar & value;
	}

	friend std::ostream& operator<<(std::ostream& out, const l2_3gpp_addr&)
	{
		return out;
	}

	bool operator==(const l2_3gpp_addr& other) const
	{
		return ((value == other.value));
	}

	octet_string value;
};

///////////////////////////////////////////////////////////////////////////////
/**
 * Define L2_3GPP2_ADDR data type.
 */
struct l2_3gpp2_addr {
	template<class ArchiveT>
	void serialize(ArchiveT& ar)
	{
		ar & value;
	}

	friend std::ostream& operator<<(std::ostream& out, const l2_3gpp2_addr&)
	{
		return out;
	}

	bool operator==(const l2_3gpp2_addr& other) const
	{
		return ((value == other.value));
	}

	octet_string value;
};

///////////////////////////////////////////////////////////////////////////////
/**
 * Define OTHER_L2_ADDR data type.
 */
struct other_l2_addr {
	template<class ArchiveT>
	void serialize(ArchiveT& ar)
	{
		ar & value;
	}

	friend std::ostream& operator<<(std::ostream& out, const other_l2_addr&)
	{
		return out;
	}

	bool operator==(const other_l2_addr& other) const
	{
		return ((value == other.value));
	}

	octet_string value;
};

///////////////////////////////////////////////////////////////////////////////
/**
 * Define LINK_ADDR data type.
 */
typedef boost::variant<mac_addr,
			l2_3gpp_3g_cell_id,
			l2_3gpp_2g_cell_id,
			l2_3gpp_addr,
			l2_3gpp2_addr,
			other_l2_addr
		      > link_addr;

/**
 * Define LIST(LINK_ADDR) data type.
 */
typedef std::vector<link_addr> link_addr_list;

///////////////////////////////////////////////////////////////////////////////
/**
 * Define IP_ADDR data type.
 */
class ip_addr : public transport_addr {
public:
	enum type_ip_enum {
		none = 0,
		ipv4 = 1,
		ipv6 = 2,
	};

	ip_addr(type_ip_enum tp = none) : transport_addr(tp)
	{ }

	ip_addr(type_ip_enum tp, const void* raw, size_t len)
		: transport_addr(tp, raw, len)
	{ }

	octet_string address() const;
	void         address(const octet_string& addr);

	friend std::ostream& operator<<(std::ostream& out, const ip_addr& tp)
	{
		out << "\ntype: " << tp.type();
		out << "\naddress: " << tp.address();

		return out;
	}

	bool operator==(const ip_addr& other) const
	{
		return ((type() == other.type()) && (address().compare(other.address()) == 0));
	}
};

/**
 * Define DHCP_SERV data type.
 */
typedef ip_addr dhcp_serv;

/**
 * Define FN_AGENT data type.
 */
typedef ip_addr fn_agent;

/**
 * Define ACC_RTR data type.
 */
typedef ip_addr acc_rtr;

///////////////////////////////////////////////////////////////////////////////
} /* namespace mih */ } /*namespace odtone */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* ODTONE_MIH_TYPES_ADDRESS__HPP_ */
