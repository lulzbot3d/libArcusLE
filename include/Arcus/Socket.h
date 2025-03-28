// Copyright (c) 2022 Ultimaker B.V.
// libArcus is released under the terms of the LGPLv3 or higher.

#ifndef ARCUS_SOCKET_H
#define ARCUS_SOCKET_H

#include <memory>

#include "Arcus/Error.h"
#include "Arcus/Types.h"

namespace Arcus
{
class SocketListener;

/**
 * \brief Threaded socket class.
 *
 * This class represents a socket and the logic for parsing and handling
 * protobuf messages that can be sent and received over this socket.
 *
 * Please see the README in libArcus for more details.
 */
class Socket
{
public:
    Socket();
    virtual ~Socket();

    /**
     * Get the socket state.
     *
     * \return The current socket state.
     */
    virtual SocketState getState() const;

    /**
     * Get the last error.
     *
     * \return The last error that occurred.
     */
    Error getLastError() const;

    /**
     * Clear any error that was set previously.
     */
    void clearError();

    /**
     * Register a new type of Message to handle.
     *
     * If the socket state is not SocketState::Initial, this method will do nothing.
     *
     * \param message_type An instance of the Message that will be used as factory object.
     *
     */
    virtual bool registerMessageType(const google::protobuf::Message* message_type);

    /**
     * Register all message types contained in a Protobuf protocol description file.
     *
     * If the socket state is not SocketState::Initial, this method will do nothing.
     *
     * \param file_name The absolute path to a Protobuf protocol file to load message types from.
     */
    virtual bool registerAllMessageTypes(const std::string& file_name);

    virtual void dumpMessageTypes();

    /**
     * Add a listener object that will be notified of socket events.
     *
     * If the socket state is not SocketState::Initial, this method will do nothing.
     *
     * \param listener The listener to add.
     */
    void addListener(SocketListener* listener);

    /**
     * Remove a listener from the list of listeners.
     *
     * If the socket state is not SocketState::Initial, this method will do nothing.
     *
     * \param listener The listener to remove.
     */
    void removeListener(SocketListener* listener);

    /**
     * Connect to an address and port.
     *
     * \param address The IP address to connect to.
     * \param port The port to connect to.
     */
    virtual void connect(const std::string& address, int port);

    /**
     * Listen for connections on an address and port.
     *
     * \param address The IP address to listen on.
     * \param port The port to listen on.
     */
    virtual void listen(const std::string& address, int port);

    /**
     * Close the connection and stop handling any messages.
     */
    virtual void close();

    /**
     * Reset a socket for re-use. State must be Closed or Error
     */
    virtual void reset();

    /**
     * Send a message across the socket.
     */
    virtual bool sendMessage(MessagePtr message);

    /**
     * Remove and return the next pending message from the queue with condition blocking.
     */
    virtual MessagePtr takeNextMessage();

    /**
     * Create an instance of a Message class.
     *
     * \param type_name The type name of the Message type to create an instance of.
     */
    virtual MessagePtr createMessage(const std::string& type_name);

private:
    // Copy and assignment is not supported.
    Socket(const Socket&);
    Socket& operator=(const Socket& other);

    class Private;
    const std::unique_ptr<Private> d;
};
} // namespace Arcus

#endif // ARCUS_SOCKET_H
