import React, { Component } from 'react';
import 'bootstrap/dist/css/bootstrap.min.css';
import { Container, Row, Col } from 'react-bootstrap';
import { BrowserRouter as Router } from "react-router-dom";
import { NavigationBar } from './components/NavigationBar';
import Sidebar from './components/Sidebar';
import SinglePlot from './components/SPlot';
import './mystyle.css'
import Client from './components/socketClient';



export default class App extends Component {

  constructor(props) {
    super(props);

    // subscribeToTimer(null, this.getUpdate);
    this.state = {
      maxLength: 100,
      plot1: [],
      plot2: [],
      plot3: [],
      plot4: [],
      plot5: []
    };

  }

  handleDataCollecting = event => {
    if (!event) {
      this.setState({
        plot1: [],
        plot2: [],
        plot3: [],
        plot4: [],
        plot5: []
      })
    }
  }

  getUpdate = (message) => {
    console.log(this.state.plot1.length, this.state.plot1)
    if (this.state.plot1.length <= this.state.maxLength) {
      this.setState({ 
        plot1: [...this.state.plot1, message.plot1],
        plot2: [...this.state.plot2, message.plot2],
        plot3: [...this.state.plot3, message.plot3],
        plot4: [...this.state.plot4, message.plot4],
        plot5: [...this.state.plot5, message.plot5],
       })
    }
    else {
      var diff = this.state.plot1.length - this.state.maxLength;
      console.log("THis is diff", diff)
      this.setState({ 
        plot1: [...this.state.plot1.slice(diff), message.plot1],
        plot2: [...this.state.plot2.slice(diff), message.plot2],
        plot3: [...this.state.plot3.slice(diff), message.plot3],
        plot4: [...this.state.plot4.slice(diff), message.plot4],
        plot5: [...this.state.plot5.slice(diff), message.plot5],
       })
    }
  };

  render() {
    return (
      <React.Fragment>
        <Router>
          <NavigationBar />
          <br />
            <Sidebar />
            <div style={{marginLeft: '100px'}}>
              <Client getUpdate={this.getUpdate} handleDataCollecting={this.handleDataCollecting}/>
              <br />
              <Container fluid>
                <Row>
                  <div className="chart_headers">
                    Heat Exchanger
                  </div>
                </Row>
                <br />

                <Row>
                  <Col>
                    <SinglePlot data={this.state.plot1} title="test 1" xaxis="x axis test" yaxis="y axis test" maxLength={this.state.maxLength}/>
                  </Col>
                  <Col>
                    <SinglePlot data={this.state.plot2} title="test 2" xaxis="x axis test" yaxis="y axis test" maxLength={this.state.maxLength}/>
                  </Col>
                  <Col>
                  </Col>
                </Row>
                <br />

                <Row>
                  <div className="chart_headers">
                    Regulator
                  </div>
                </Row>
                <br />

                <Row>
                  <Col>
                    <SinglePlot data={this.state.plot3} title="building 1" xaxis="x axis test" yaxis="y axis test" maxLength={this.state.maxLength}/>
                  </Col>
                  <Col>
                    <SinglePlot data={this.state.plot4} title="building 2" xaxis="x axis test" yaxis="y axis test" maxLength={this.state.maxLength} color="#0066cc"/>
                  </Col>
                  <Col>
                    <SinglePlot data={this.state.plot5} title="building 3" xaxis="x axis test" yaxis="y axis test" maxLength={this.state.maxLength} color="#99cc00"/>
                  </Col>
                </Row>
                <br />
              </Container>
            </div>
        </Router>
      </React.Fragment>
    );
  }
}