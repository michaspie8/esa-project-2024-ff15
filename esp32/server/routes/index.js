const express = require('express');
const router = express.Router();
const path = require('path');
const fs = require('fs');
/* GET home page. */
router.get('/', function (req, res, next)
{
    res.render('index');
});
router.get('/tick', function (req, res, next)
{
    //get from count file
    let count = 0;
    try
    {
        count = parseInt
        (
            fs.readFileSync(path.join(__dirname, '../count')).toString()
        );
    }
    catch (e)
    {
        console.log(e);
    }
    res.send({impulse: count});
});
router.put('/tick/:impulse', function (req, res, next){
    //write to count file
    if(isNaN(req.params.impulse))
    {
        res.status(400).send({error: 'Invalid impulse'});
        return;
    }
    fs.writeFileSync(path.join(__dirname, '../count'), req.params.impulse);
    res.send({impulse: req.params.impulse});
});
module.exports = router;
