//
// Copyright (c) 2016-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

// Test that header file is self-contained.
#include <boost/beast/core/type_traits.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/detail/consuming_buffers.hpp>
#include <memory>

namespace boost {
namespace beast {

namespace detail {

namespace {

//
// is_invocable
//

struct is_invocable_udt1
{
    void operator()(int) const;
};

struct is_invocable_udt2
{
    int operator()(int) const;
};

struct is_invocable_udt3
{
    int operator()(int);
};

struct is_invocable_udt4
{
    void operator()(std::unique_ptr<int>);
};

#ifndef __INTELLISENSE__
// VFALCO Fails to compile with Intellisense
BOOST_STATIC_ASSERT(is_invocable<is_invocable_udt1, void(int)>::value);
BOOST_STATIC_ASSERT(is_invocable<is_invocable_udt2, int(int)>::value);
BOOST_STATIC_ASSERT(is_invocable<is_invocable_udt3, int(int)>::value);
BOOST_STATIC_ASSERT(! is_invocable<is_invocable_udt1, void(void)>::value);
BOOST_STATIC_ASSERT(! is_invocable<is_invocable_udt2, int(void)>::value);
BOOST_STATIC_ASSERT(! is_invocable<is_invocable_udt2, void(void)>::value);
BOOST_STATIC_ASSERT(! is_invocable<is_invocable_udt3 const, int(int)>::value);
BOOST_STATIC_ASSERT(is_invocable<is_invocable_udt4, void(std::unique_ptr<int>)>::value);
#endif

} // (anonymous)

} // detail

//
// handler concepts
//

namespace {

struct H
{
    void operator()(int);
};

} // anonymous

BOOST_STATIC_ASSERT(is_completion_handler<H, void(int)>::value);
BOOST_STATIC_ASSERT(! is_completion_handler<H, void(void)>::value);

//
// stream concepts
//

namespace {

using stream_type = net::ip::tcp::socket;

struct not_a_stream
{
    void
    get_io_service();
};

BOOST_STATIC_ASSERT(has_get_executor<stream_type>::value);
BOOST_STATIC_ASSERT(is_async_read_stream<stream_type>::value);
BOOST_STATIC_ASSERT(is_async_write_stream<stream_type>::value);
BOOST_STATIC_ASSERT(is_async_stream<stream_type>::value);
BOOST_STATIC_ASSERT(is_sync_read_stream<stream_type>::value);
BOOST_STATIC_ASSERT(is_sync_write_stream<stream_type>::value);
BOOST_STATIC_ASSERT(is_sync_stream<stream_type>::value);

BOOST_STATIC_ASSERT(! has_get_executor<not_a_stream>::value);
BOOST_STATIC_ASSERT(! is_async_read_stream<not_a_stream>::value);
BOOST_STATIC_ASSERT(! is_async_write_stream<not_a_stream>::value);
BOOST_STATIC_ASSERT(! is_sync_read_stream<not_a_stream>::value);
BOOST_STATIC_ASSERT(! is_sync_write_stream<not_a_stream>::value);

} // (anonymous)

} // beast
} // boost
