import React, { Component } from 'react';
import { Chart } from 'react-charts'
import Grid from '@material-ui/core/Grid';
import Paper from '@material-ui/core/Paper';
import '../mystyle.css'



class SinglePlot extends Component {

    constructor(props) {
        super(props)
    
        this.state = {
          data: props.data,
          title: props.title,
          xaxis: props.xaxis,
          yaxis: props.yaxis,
          maxLength: props.maxLength,
          color: props.color || "#cc7a00"
        }
    }

    componentWillReceiveProps(props) {
        this.setState ({ data: props.data })
    }


    createPlot() {
        const axes = [
            { primary: true, type: 'linear', position: 'bottom', id: 'test'},
            { type: 'linear', position: 'left'},
        ];

        const data = [
            {
                label: this.state.title,
                data: this.prepareData(this.state.data)
            }
        ];

        const style = series => ({
            color: this.state.color
        });

        return (
            <div
                style={{
                    width: '400px',
                    height: '300px'
                }}
                >
                    <Chart data={data} axes={axes} getSeriesStyle={style}/>
            </div>
        )
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
            resu[i] = [i, data[i + start]];
        }
        return resu;
    }

    render() {

        return (
                <Grid container spacing={0} width="400px">
                    <Grid item className='rotated'>
                        {this.state.yaxis}
                    </Grid>
                    <Grid item xs={11}>
                        <Paper square='true' style={{width: '400px', background: '#b3b3b3'}}>
                            <div style={{textAlign: 'center'}}>
                                {this.state.title}
                                {this.createPlot()}
                                {this.state.xaxis}
                            </div>
                        </Paper>
                    </Grid>
                </Grid>
        );
    }
}
export default SinglePlot