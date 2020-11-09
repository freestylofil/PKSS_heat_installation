import React, { Component } from "react";
import { Button } from '@material-ui/core';
import io from 'socket.io-client';
import '../mystyle.css'


class Client extends Component {

    constructor(props) {
        super(props);
        this.state= { collectData: false }
    }


    getUpdate = message => {
        this.props.getUpdate(message);
    };

    handleDataCollecting() {
        if (this.state.collectData) {
            this.props.handleDataCollecting(this.state.collectData)
            this.stopConnection()
            this.setState({ collectData: false })
        } else {
            this.props.handleDataCollecting(this.state.collectData)
            this.startConnection()
            this.setState({ collectData: true })
        }
    }
    
    componentWillUnmount() {
        this.stopConnection()
        console.log("component unmounted")
    }

    componentDidMount() {
        // this.startConnection();
    }


    stopConnection() {
        this.socket.close()
    }

    startConnection() {
        var serverEndpoint = "http://localhost:5000"
            this.socket = io.connect(serverEndpoint, {
                reconnection: true
        });
        console.log("component mounted")
        this.socket.on("responseMessage", message => {
            console.log("This is response message:", message)
            this.getUpdate(message.data)
        }) 
    }

    render() {
        return (
            <div className="collect_button">
                <Button variant="outlined" color='inherit' onClick={() => { this.handleDataCollecting() }}>
                    {this.state.collectData ? "Stop collecting" : "Start collecting"}
                </Button>
            </div>
        )
    }
}
export default Client