import CanvasJSReact from '../lib/canvasjs.react';
import React, { Component } from 'react';
import { Chart } from 'react-charts'
import Grid from '@material-ui/core/Grid';
import Paper from '@material-ui/core/Paper';
import '../mystyle.css'

var CanvasJS = CanvasJSReact.CanvasJS;
var CanvasJSChart = CanvasJSReact.CanvasJSChart;

var dps = [{x: 1, y: 10}, {x: 2, y: 13}, {x: 3, y: 18}, {x: 4, y: 20}, {x: 5, y: 17},{x: 6, y: 10}, {x: 7, y: 13}, {x: 8, y: 18}, {x: 9, y: 20}, {x: 10, y: 17}];   //dataPoints.
var xVal = dps.length + 1;
var yVal = 15;
var updateInterval = 1000;


class SinglePlot extends Component {

    constructor(props) {
        super(props);
        this.updateChart = this.updateChart.bind(this);
        
        this.state = {
            data: props.data,
            maxLength: props.maxLength,
            title: props.title,
            xaxis: props.xaxis,
            yaxis: props.yaxis,
            color: props.color || "#cc7a00"
        }
    }
    
	componentDidMount() {
		setInterval(this.updateChart, updateInterval);
    }
    
    componentWillReceiveProps(props) {
        this.setState ({ data: props.data })
    }

    prepareData(data) {
        var i;
        var resu = [];
        var start;
        if (data.length < this.state.maxLength) {
            start = 0;
        } else {
            start = data.length - this.state.maxLength;
        }
        for (i = 0; i < data.length - start; i++) {
            resu[i] = {x: i, y: data[i + start]};
        }
        return resu;
    }

    updateChart() {
		this.chart.render();
    }
    
	render() {
		const options = {
			title :{
				text: this.state.title
            },
            backgroundColor: '#b3b3b3',
			data: [{
				type: "line",
                dataPoints : this.prepareData(this.state.data),
                lineColor: this.state.color,
                markerColor: this.state.color
            }],
            axisX:{
                title : this.state.xaxis
            },            
            axisY:{
                title : this.state.yaxis
            },
        }
        
		return (
		<div>
			<CanvasJSChart options = {options}
				 onRef={ref => this.chart = ref}
			/>
			{/*You can get reference to the chart instance as shown above using onRef. This allows you to access all chart properties and methods*/}
		</div>
		);
	}
}
export default SinglePlot